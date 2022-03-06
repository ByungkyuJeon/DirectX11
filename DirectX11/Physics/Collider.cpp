#include "Collider.h"

void Collider::setType(Collider::Type type)
{
	this->mType = type;
}

Collider::Type Collider::getType() const
{
	return this->mType;
}
