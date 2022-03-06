#pragma once

#include "../CoordinateSystem/Transform.h"
#include <memory>
#include <DirectXMath.h>

class PhysicsObject
{
public:
	PhysicsObject() = default;
	PhysicsObject(std::shared_ptr<Transform> transform, std::shared_ptr<DirectX::XMFLOAT3> velocity);

	void setTransform(std::shared_ptr<Transform> transform);
	void setVelocity(std::shared_ptr<DirectX::XMFLOAT3> velocity);

	std::shared_ptr<Transform> getTransform() const;
	std::shared_ptr<DirectX::XMFLOAT3> getVelocity() const;

	void Update(float delta);

private:
	std::shared_ptr<Transform> mTransform;
	std::shared_ptr<DirectX::XMFLOAT3> mVelocity;
};