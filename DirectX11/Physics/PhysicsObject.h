#pragma once

#include <DirectXMath.h>

class PhysicsObject
{
public:
	PhysicsObject() = default;
	PhysicsObject(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& velocity);
	PhysicsObject(const DirectX::XMVECTOR& position, const DirectX::XMVECTOR& velocity);

	const DirectX::XMFLOAT3& getPosition() const;
	const DirectX::XMFLOAT3& getVelocity() const;

	void setPosition(const DirectX::XMFLOAT3& position);
	void setPosition(const DirectX::XMVECTOR& position);
	void setVelocity(const DirectX::XMFLOAT3& velocity);
	void setVelocity(const DirectX::XMVECTOR& velocity);

	void Update(float delta);

private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 velocity;
};