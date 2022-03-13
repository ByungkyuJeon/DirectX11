#pragma once

#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine()
{
	
}

bool PhysicsEngine::Initialize()
{
	return true;
}

void PhysicsEngine::registerPhysicsObject(std::shared_ptr<PhysicsObject> physicsObject)
{
	// 환경 물리 연산 대상에 추가
	this->mPhysicsObjects.emplace_back(physicsObject);

	// 충돌 검사 연산 대상에 추가
	if (!physicsObject->getGameObject()->getGameObjectSetting().CollisionDetectionEnabled())
	{
		return;
	}

	if (physicsObject->getGameObject()->getCollider() == nullptr)
	{
		return;
	}

	this->mCollisionObjects.emplace_back(physicsObject);
}

void PhysicsEngine::registerPhysicsObject(std::shared_ptr<GameObject> gameObject)
{
	std::shared_ptr<PhysicsObject> physicsObject = std::make_shared<PhysicsObject>(gameObject);

	// 환경 물리 연산 대상에 추가
	this->mPhysicsObjects.emplace_back(physicsObject);

	// 충돌 검사 연산 대상에 추가
	if (!physicsObject->getGameObject()->getGameObjectSetting().CollisionDetectionEnabled())
	{
		return;
	}

	if (physicsObject->getGameObject()->getCollider() == nullptr)
	{
		return;
	}

	this->mCollisionObjects.emplace_back(physicsObject);
}

size_t PhysicsEngine::getObjectSize() const
{
	return this->mPhysicsObjects.size();
}

std::shared_ptr<PhysicsObject> PhysicsEngine::getObject(size_t idx) const
{
	return this->mPhysicsObjects[idx];
}

void PhysicsEngine::update(float delta)
{
	if (delta > 100) { return; }

	// 충돌 검사
	this->detectCollision();

	for (auto& object : this->mPhysicsObjects)
	{
		if (object->getGameObject()->getGameObjectSetting().IsRigidBody()) { continue; }
		object->getGameObject()->addForce(0.00000001f * delta, -0.00098f * delta, 0.0f);
		object->getGameObject()->multForce(0.9f);
		object->update(delta);
	}
}

void PhysicsEngine::detectCollision()
{
	bool hasRigid;

	for (int lhsIdx = 0; lhsIdx < this->mCollisionObjects.size(); lhsIdx++)
	{
		for (int rhsIdx = lhsIdx + 1; rhsIdx < this->mCollisionObjects.size(); rhsIdx++)
		{
			if (this->mCollisionObjects[lhsIdx]->getGameObject()->getGameObjectSetting().IsRigidBody()
				&& this->mCollisionObjects[rhsIdx]->getGameObject()->getGameObjectSetting().IsRigidBody())
			{
				continue;
			}
			hasRigid = this->mCollisionObjects[lhsIdx]->getGameObject()->getGameObjectSetting().IsRigidBody()
				|| this->mCollisionObjects[rhsIdx]->getGameObject()->getGameObjectSetting().IsRigidBody();

			// 충돌 검사
			IntersectionData intersectionData =
				this->mCollisionObjects[lhsIdx]->getCollider()->isIntersected(this->mCollisionObjects[rhsIdx]->getCollider().get());

			if (!intersectionData.getIntersectionState())
			{
				continue;
			}

			// 충돌 이벤트 전파
			this->mCollisionObjects[lhsIdx]->getGameObject()->onCollided(intersectionData, this->mCollisionObjects[rhsIdx]->getCollider());
			this->mCollisionObjects[rhsIdx]->getGameObject()->onCollided(intersectionData, this->mCollisionObjects[lhsIdx]->getCollider());


			// 충돌 물리 연산
			std::shared_ptr<DirectX::XMFLOAT3> lhsVel = this->mCollisionObjects[lhsIdx]->getGameObject()->getVelocity();
			std::shared_ptr<DirectX::XMFLOAT3> rhsVel = this->mCollisionObjects[rhsIdx]->getGameObject()->getVelocity();

			// 1. 강체 체크
			// 2. 충돌 속도 크기 계산
			DirectX::XMVECTOR impactVelocity = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(lhsVel.get()), DirectX::XMLoadFloat3(rhsVel.get()));

			// 3. 질량, 방향 기반 속도 크기 분배
			DirectX::XMFLOAT3 velRatio;

			// 충돌 속도 비율 결정 - X 벡터
			if ((hasRigid
				|| lhsVel->x == 0 || rhsVel->x == 0
				|| lhsVel->x > 0 && rhsVel->x > 0)
				|| lhsVel->x < 0 && rhsVel->x < 0)
			{
				velRatio.x = lhsVel->x > rhsVel->x ? 1.0f : 0.0f;
			}
			else
			{
				velRatio.x = std::abs(lhsVel->x) / std::abs(DirectX::XMVectorGetX(impactVelocity));
			}

			// 충돌 속도 비율 결정 - Y 벡터
			if ((hasRigid
				|| lhsVel->y == 0 || rhsVel->y == 0
				|| lhsVel->y >= 0 && rhsVel->y >= 0)
				|| lhsVel->y < 0 && rhsVel->y < 0)
			{
				velRatio.y = lhsVel->y > rhsVel->y ? 1.0f : 0.0f;
			}
			else
			{
				velRatio.y = std::abs(lhsVel->y) / std::abs(DirectX::XMVectorGetY(impactVelocity));
			}

			// 충돌 속도 비율 결정 - Z 벡터
			if ((hasRigid
				|| lhsVel->y == 0 || rhsVel->y == 0
				|| lhsVel->z >= 0 && rhsVel->z >= 0)
				|| lhsVel->z < 0 && rhsVel->z < 0)
			{
				velRatio.z = lhsVel->z > rhsVel->z ? 1.0f : 0.0f;
			}
			else
			{
				velRatio.z = std::abs(lhsVel->z) / std::abs(DirectX::XMVectorGetZ(impactVelocity));
			}

			// 질량 속도 비율 결정
			float massRatio = 2.0f * this->mCollisionObjects[lhsIdx]->getGameObject()->getMass() / (this->mCollisionObjects[lhsIdx]->getGameObject()->getMass() + this->mCollisionObjects[rhsIdx]->getGameObject()->getMass());

			// 4. 방향 추가 및 업데이트
			if (hasRigid)
			{
				if (this->mCollisionObjects[rhsIdx]->getGameObject()->getGameObjectSetting().IsRigidBody())
				{
					this->mCollisionObjects[lhsIdx]->getGameObject()->getTransform()->translate
					(
						-intersectionData.getLtoRNormed().x * std::abs(intersectionData.getIntersectionDistance()),
						-intersectionData.getLtoRNormed().y * std::abs(intersectionData.getIntersectionDistance()),
						-intersectionData.getLtoRNormed().z * std::abs(intersectionData.getIntersectionDistance())
					);

					this->mCollisionObjects[lhsIdx]->getGameObject()->addForce(
						-intersectionData.getLtoRNormed().x * std::abs(DirectX::XMVectorGetX(impactVelocity)) * 2.0f,
						-intersectionData.getLtoRNormed().y * std::abs(DirectX::XMVectorGetY(impactVelocity)) * 2.0f,
						-intersectionData.getLtoRNormed().z * std::abs(DirectX::XMVectorGetZ(impactVelocity)) * 2.0f
					);
				}

				if (this->mCollisionObjects[lhsIdx]->getGameObject()->getGameObjectSetting().IsRigidBody())
				{
					this->mCollisionObjects[rhsIdx]->getGameObject()->getTransform()->translate
					(
						intersectionData.getLtoRNormed().x * std::abs(intersectionData.getIntersectionDistance()),
						intersectionData.getLtoRNormed().y * std::abs(intersectionData.getIntersectionDistance()),
						intersectionData.getLtoRNormed().z * std::abs(intersectionData.getIntersectionDistance())
					);
					
					this->mCollisionObjects[rhsIdx]->getGameObject()->addForce(
						intersectionData.getLtoRNormed().x * std::abs(DirectX::XMVectorGetX(impactVelocity)) * 2.0f,
						intersectionData.getLtoRNormed().y * std::abs(DirectX::XMVectorGetY(impactVelocity)) * 2.0f,
						intersectionData.getLtoRNormed().z * std::abs(DirectX::XMVectorGetZ(impactVelocity)) * 2.0f
					);
				}
			}
			else
			{
				this->mCollisionObjects[lhsIdx]->getGameObject()->getTransform()->translate
				(
					-intersectionData.getLtoRNormed().x * intersectionData.getIntersectionDistance() / 2,
					-intersectionData.getLtoRNormed().y * intersectionData.getIntersectionDistance() / 2,
					-intersectionData.getLtoRNormed().z * intersectionData.getIntersectionDistance() / 2
				);

				this->mCollisionObjects[lhsIdx]->getGameObject()->addForce(
					-intersectionData.getLtoRNormed().x * (1.0f - velRatio.x) * (1.0f - massRatio) * std::abs(DirectX::XMVectorGetX(impactVelocity)) * 2.0f,
					-intersectionData.getLtoRNormed().y * (1.0f - velRatio.y) * (1.0f - massRatio) * std::abs(DirectX::XMVectorGetY(impactVelocity)) * 2.0f,
					-intersectionData.getLtoRNormed().z * (1.0f - velRatio.z) * (1.0f - massRatio) * std::abs(DirectX::XMVectorGetZ(impactVelocity)) * 2.0f
				);

				this->mCollisionObjects[rhsIdx]->getGameObject()->getTransform()->translate
				(
					intersectionData.getLtoRNormed().x * intersectionData.getIntersectionDistance() / 2,
					intersectionData.getLtoRNormed().y * intersectionData.getIntersectionDistance() / 2,
					intersectionData.getLtoRNormed().z * intersectionData.getIntersectionDistance() / 2
				);

				this->mCollisionObjects[rhsIdx]->getGameObject()->addForce(
					intersectionData.getLtoRNormed().x * velRatio.x * massRatio * std::abs(DirectX::XMVectorGetX(impactVelocity)) * 2.0f,
					intersectionData.getLtoRNormed().y * velRatio.y * massRatio * std::abs(DirectX::XMVectorGetY(impactVelocity)) * 2.0f,
					intersectionData.getLtoRNormed().z * velRatio.z * massRatio * std::abs(DirectX::XMVectorGetZ(impactVelocity)) * 2.0f
				);
			}
			
		}
	}
}