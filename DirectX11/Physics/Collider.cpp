#include "Collider.h"

void Collider::setTransform(float x, float y, float z)
{
}

void Collider::setType(Collider::Type type)
{
	this->mType = type;
}

Collider::Type Collider::getType() const
{
	return this->mType;
}
