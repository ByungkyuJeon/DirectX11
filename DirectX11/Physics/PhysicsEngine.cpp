#pragma once

#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine()
{
	
}

bool PhysicsEngine::Initialize()
{
	return true;
}

void PhysicsEngine::registerPhysicsObject(const PhysicsObject& object)
{
	this->physicsObjects.emplace_back(object);
}

void PhysicsEngine::registerPhysicsObject(std::shared_ptr<GameObject> gameObject)
{
	this->physicsObjects.emplace_back(PhysicsObject(gameObject));
}

size_t PhysicsEngine::getObjectSize() const
{
	return this->physicsObjects.size();
}

const PhysicsObject& PhysicsEngine::getObject(size_t idx) const
{
	return this->physicsObjects[idx];
}

void PhysicsEngine::Update(float delta)
{
	for (auto& object : this->physicsObjects)
	{
		object.update(delta);
	}
}
