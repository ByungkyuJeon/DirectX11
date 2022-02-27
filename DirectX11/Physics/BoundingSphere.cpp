#include "BoundingSphere.h"

const DirectX::XMFLOAT3& BoundingSphere::getCenter()
{
	return this->center;
}

float BoundingSphere::getRadius()
{
	return this->radius;
}

void BoundingSphere::setCenter(DirectX::XMFLOAT3& center)
{
	this->center = center;
}

void BoundingSphere::setCenter(DirectX::XMVECTOR& center)
{
	DirectX::XMStoreFloat3(&this->center, center);
}

void BoundingSphere::setRadius(float radius)
{
	this->radius = radius;
}

IntersectionData BoundingSphere::isIntersected(const BoundingSphere& other) const
{
	IntersectionData ret;
	float dist = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&this->center), DirectX::XMLoadFloat3(&other.center))));
	bool intersectionState = dist <= this->radius + other.radius;
	ret.setIntersectionState(intersectionState);
	if (intersectionState)
	{
		ret.setIntersectionDistance((this->radius + other.radius) - dist);
	}
	return ret;
}
