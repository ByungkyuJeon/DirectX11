#include "GameObject.h"

void GameObject::onCollided(const IntersectionData& intersectionData, std::shared_ptr<Collider> other)
{

}

void GameObject::update(float delta)
{
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

void GameObject::setAcceleration(const DirectX::XMFLOAT3& acceleration)
{
	this->mAcceleration = acceleration;
}

void GameObject::setMass(float mass)
{
	this->mMass = mass;
}

const GameObjectSetting& GameObject::getGameObjectSetting() const
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

const DirectX::XMFLOAT3 GameObject::getAcceleration() const
{
	return this->mAcceleration;
}

float GameObject::getMass() const
{
	return this->mMass;
}

void GameObject::accelerate(const DirectX::XMFLOAT3& acceleration)
{
	this->mAcceleration.x += acceleration.x;
	this->mAcceleration.y += acceleration.y;
	this->mAcceleration.z += acceleration.z;
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

bool GameObjectSetting::IsHiddenInGame() const
{
	return this->mIsHiddenInGame;
}

bool GameObjectSetting::CollisionDetectionEnabled() const
{
	return this->mCollisionDetectionEnabled;
}
