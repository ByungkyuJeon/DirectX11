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
	// 충돌 검사
	this->detectCollision();

	for (auto& object : this->mPhysicsObjects)
	{
		object->update(delta);
	}
}

void PhysicsEngine::detectCollision()
{
	for (int lhsIdx = 0; lhsIdx < this->mCollisionObjects.size(); lhsIdx++)
	{
		for (int rhsIdx = lhsIdx; rhsIdx < this->mCollisionObjects.size(); rhsIdx++)
		{
			IntersectionData intersectionData =
				this->mPhysicsObjects[lhsIdx]->getCollider()->isIntersected(this->mPhysicsObjects[rhsIdx]->getCollider().get());

			if (!intersectionData.getIntersectionState())
			{
				continue;
			}

			// 충돌 이벤트 전파
			this->mPhysicsObjects[lhsIdx]->getGameObject()->onCollided(intersectionData, this->mPhysicsObjects[rhsIdx]->getCollider());
			this->mPhysicsObjects[rhsIdx]->getGameObject()->onCollided(intersectionData, this->mPhysicsObjects[lhsIdx]->getCollider());

			// 충돌 물리 연산
			
		}
	}
}