#include "GameObject.h"

void GameObject::update(float delta)
{
	this->updateMatrix();
}

GameObject::GameObject(std::shared_ptr<Transform> transform)
{
	this->mTransform = transform;
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

void GameObject::updateMatrix()
{
	if (this->mTransform != nullptr)
	{
		this->mTransform->updateMatrix();
	}
}
