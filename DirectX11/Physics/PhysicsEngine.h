#pragma once

#include "PhysicsObject.h"
#include <vector>


class PhysicsEngine
{
public:
	PhysicsEngine();

	bool Initialize();

	void registerObject(const PhysicsObject& object);
	//bool removeObject(const PhysicsObject& object);

	size_t getObjectSize() const;
	const PhysicsObject& getObject(size_t idx) const;

	void Update(float delta);

private:
	std::vector<PhysicsObject> physicsObjects;
};