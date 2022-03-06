#include "GameObject.h"

void GameObject::update()
{
	this->updateMatrix();
}

void GameObject::setTransform(std::shared_ptr<Transform> transform)
{
	this->mTransform = transform;
}

void GameObject::setModel(std::shared_ptr<Model> model)
{
	this->mModel = model;
}

std::shared_ptr<Transform> GameObject::getTransform() const
{
	return this->mTransform;
}

std::shared_ptr<Model> GameObject::getModel() const
{
	return this->mModel;
}

void GameObject::updateMatrix()
{
	if (this->mTransform != nullptr)
	{
		this->mTransform->updateMatrix();
	}
}
