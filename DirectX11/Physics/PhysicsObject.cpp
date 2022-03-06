#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(std::shared_ptr<Transform> transform, std::shared_ptr<DirectX::XMFLOAT3> velocity) :
	mTransform{ transform }, mVelocity{ velocity }
{
}

void PhysicsObject::setTransform(std::shared_ptr<Transform> transform)
{
	this->mTransform = transform;
}

void PhysicsObject::setVelocity(std::shared_ptr<DirectX::XMFLOAT3> velocity)
{
	this->mVelocity = velocity;
}

std::shared_ptr<Transform> PhysicsObject::getTransform() const
{
	return this->mTransform;
}

std::shared_ptr<DirectX::XMFLOAT3> PhysicsObject::getVelocity() const
{
	return this->mVelocity;
}

void PhysicsObject::Update(float delta)
{
	DirectX::XMFLOAT3 res;
	DirectX::XMStoreFloat3(
		&res,
		DirectX::XMVectorMultiply(
			DirectX::XMLoadFloat3(this->mVelocity.get()),
			DirectX::XMVectorReplicate(delta)
		));

	this->mTransform->setPosition(res);
}
