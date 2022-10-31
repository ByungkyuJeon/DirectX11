#include "GameObject.h"

void GameObject::onCollided(const IntersectionData& intersectionData, std::shared_ptr<Collider> other)
{

}

void GameObject::update(float delta)
{
	if (this->mCollider != nullptr)
	{
		this->mCollider->setTransform
		(
			this->mTransform->getWorldPosition().x,
			this->mTransform->getWorldPosition().y,
			this->mTransform->getWorldPosition().z
		);
	}

	this->updateMatrix();
}

GameObject::GameObject(std::shared_ptr<Transform> transform)
{
	this->mTransform = transform;
}

GameObject::GameObject(std::shared_ptr<Transform> transform, std::shared_ptr<Model> model, std::shared_ptr<DirectX::XMFLOAT3> velocity) :
	mTransform{ transform }, mModel{ model }, mVelocity{ velocity }
{
}

void GameObject::setTransform(std::shared_ptr<Transform> transform)
{
	this->mTransform = transform;
}

void GameObject::setModel(std::shared_ptr<Model> model)
{
	this->mModel = model;
}

void GameObject::setVelocity(std::shared_ptr<DirectX::XMFLOAT3> velocity)
{
	this->mVelocity = velocity;
}

void GameObject::setCollider(std::shared_ptr<Collider> collider)
{
	this->mCollider = collider;
}

void GameObject::setMass(float mass)
{
	this->mMass = mass;
}

GameObjectSetting& GameObject::getGameObjectSetting()
{
	return this->mGameObjectSetting;
}

std::shared_ptr<Transform> GameObject::getTransform() const
{
	return this->mTransform;
}

std::shared_ptr<Model> GameObject::getModel() const
{
	return this->mModel;
}

std::shared_ptr<DirectX::XMFLOAT3> GameObject::getVelocity() const
{
	return this->mVelocity;
}

std::shared_ptr<Collider> GameObject::getCollider() const
{
	return this->mCollider;
}

float GameObject::getMass() const
{
	return this->mMass;
}

void GameObject::addForce(const DirectX::XMFLOAT3& force)
{
	this->mVelocity->x += force.x;
	this->mVelocity->y += force.y;
	this->mVelocity->z += force.z;
}

void GameObject::addForce(float x, float y, float z)
{
	this->mVelocity->x += x;
	this->mVelocity->y += y;
	this->mVelocity->z += z;
}

void GameObject::multForce(float mult)
{
	this->mVelocity->x *= mult;
	this->mVelocity->y *= mult;
	this->mVelocity->z *= mult;
}

void GameObject::updateMatrix()
{
	if (this->mTransform != nullptr)
	{
		this->mTransform->updateMatrix();
	}
}

void GameObjectSetting::setCollisionDetectionState(bool state)
{
	this->mCollisionDetectionEnabled = state;
}

void GameObjectSetting::setHiddenInGameState(bool state)
{
	this->mIsHiddenInGame = state;
}

void GameObjectSetting::setRigidBodyState(bool state)
{
	this->mIsRigidBody = state;
}

bool GameObjectSetting::IsHiddenInGame() const
{
	return this->mIsHiddenInGame;
}

bool GameObjectSetting::CollisionDetectionEnabled() const
{
	return this->mCollisionDetectionEnabled;
}

bool GameObjectSetting::IsRigidBody() const
{
	return this->mIsRigidBody;
}
