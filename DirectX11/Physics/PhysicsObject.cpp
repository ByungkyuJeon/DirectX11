#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& velocity) :
	position{ position }, velocity{ velocity }
{
}

PhysicsObject::PhysicsObject(const DirectX::XMVECTOR& position, const DirectX::XMVECTOR& velocity)
{
	DirectX::XMStoreFloat3(&this->position, position);
	DirectX::XMStoreFloat3(&this->velocity, velocity);
}

const DirectX::XMFLOAT3& PhysicsObject::getPosition() const
{
	return this->position;
}

const DirectX::XMFLOAT3& PhysicsObject::getVelocity() const
{
	return this->velocity;
}

void PhysicsObject::setPosition(const DirectX::XMFLOAT3& position)
{
	this->position = position;
}

void PhysicsObject::setPosition(const DirectX::XMVECTOR& position)
{
	DirectX::XMStoreFloat3(&this->position, position);
}

void PhysicsObject::setVelocity(const DirectX::XMFLOAT3& velocity)
{
	this->velocity = velocity;
}

void PhysicsObject::setVelocity(const DirectX::XMVECTOR& velocity)
{
	DirectX::XMStoreFloat3(&this->velocity, velocity);
}

void PhysicsObject::Integrate(float delta)
{
	DirectX::XMStoreFloat3(
		&this->position,
		DirectX::XMVectorMultiply(
			DirectX::XMLoadFloat3(&this->velocity),
			DirectX::XMVectorReplicate(delta)
		));
}
