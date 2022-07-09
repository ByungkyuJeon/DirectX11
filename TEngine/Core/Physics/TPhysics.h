#pragma once

#include "../Core.h"
#include "PhysicsObject.h"

struct PhysicsEngineSetting
{
public:

private:
	DirectX::XMFLOAT3 mGravity;
};

class TPhysics
{
public:
	TPhysics() {};

	bool Initialize();

	void registerPhysicsObject(std::shared_ptr<PhysicsObject> physicsObject);
	void registerPhysicsObject(std::shared_ptr<GameObject> gameObject);
	//bool removeObject(const PhysicsObject& object);

	size_t getObjectSize() const;
	std::shared_ptr<PhysicsObject> getObject(size_t idx) const;

	void update(float delta);

private:
	std::vector<std::shared_ptr<PhysicsObject>> mPhysicsObjects;
	std::vector<std::shared_ptr<PhysicsObject>> mCollisionObjects;

	void detectCollision();
};