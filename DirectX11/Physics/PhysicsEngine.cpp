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
	// ȯ�� ���� ���� ��� �߰�
	this->mPhysicsObjects.emplace_back(physicsObject);

	// �浹 �˻� ���� ��� �߰�
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

	// ȯ�� ���� ���� ��� �߰�
	this->mPhysicsObjects.emplace_back(physicsObject);

	// �浹 �˻� ���� ��� �߰�
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
	// �浹 �˻�
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

			// �浹 �̺�Ʈ ����
			this->mPhysicsObjects[lhsIdx]->getGameObject()->onCollided(intersectionData, this->mPhysicsObjects[rhsIdx]->getCollider());
			this->mPhysicsObjects[rhsIdx]->getGameObject()->onCollided(intersectionData, this->mPhysicsObjects[lhsIdx]->getCollider());

			// �浹 ���� ����
			
		}
	}
}