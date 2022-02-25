#include "BoundingSphere.h"

const DirectX::XMVECTOR& BoundingSphere::getCenter()
{
	return center;
}

float BoundingSphere::getRadius()
{
	return radius;
}

void BoundingSphere::setCenter(DirectX::XMVECTOR& center)
{
	this->center = center;
}

void BoundingSphere::setRadius(float radius)
{
	this->radius = radius;
}

IntersectionData BoundingSphere::isIntersected(const BoundingSphere& other) const
{
	IntersectionData ret;
	float dist = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorAdd(this->center, other.center)));
	bool intersectionState = dist <= this->radius + other.radius;
	ret.setIntersectionState(intersectionState);
	if (intersectionState)
	{
		ret.setIntersectionDistance((this->radius + other.radius) - dist);
	}
	return ret;
}
