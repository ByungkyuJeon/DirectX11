#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(std::shared_ptr<GameObject> gameObject) :
	mGameObject{ gameObject }
{
}

std::shared_ptr<GameObject> PhysicsObject::getGameObject() const 
{
	return this->mGameObject;
}

std::shared_ptr<Transform> PhysicsObject::getTransform() const
{
	return this->mGameObject->getTransform();
}

std::shared_ptr<DirectX::XMFLOAT3> PhysicsObject::getVelocity() const
{
	return this->mGameObject->getVelocity();
}

std::shared_ptr<Collider> PhysicsObject::getCollider() const
{
	return this->mGameObject->getCollider();
}

void PhysicsObject::update(float delta)
{
	DirectX::XMFLOAT3 res;

	DirectX::XMStoreFloat3(
		&res,
		DirectX::XMVectorMultiply(
			DirectX::XMLoadFloat3(this->mGameObject->getVelocity().get()),
			DirectX::XMVectorReplicate(delta)
		));

	this->mGameObject->getTransform()->translate(res);
}
