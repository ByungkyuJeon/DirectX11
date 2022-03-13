#pragma once

#include "../GameSystem/GameObject.h"
#include <memory>
#include <DirectXMath.h>

class PhysicsObject
{
public:
	PhysicsObject() = default;
	PhysicsObject(std::shared_ptr<GameObject> gameObject);

	std::shared_ptr<GameObject> getGameObject() const;
	std::shared_ptr<Transform> getTransform() const;
	std::shared_ptr<DirectX::XMFLOAT3> getVelocity() const;
	std::shared_ptr<Collider> getCollider() const;

	void update(float delta);

private:
	std::shared_ptr<GameObject> mGameObject;
};