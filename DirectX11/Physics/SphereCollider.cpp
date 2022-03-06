#include "SphereCollider.h"

SphereCollider::SphereCollider(DirectX::XMVECTOR& center, float radius) :
	radius{ radius }
{
	DirectX::XMStoreFloat3(&this->center, center);
}

SphereCollider::SphereCollider(DirectX::XMFLOAT3& center, float radius) :
	center{ center }, radius{ radius }
{
}

const DirectX::XMFLOAT3& SphereCollider::getCenter() const
{
	return this->center;
}

float SphereCollider::getRadius() const
{
	return this->radius;
}

void SphereCollider::setCenter(DirectX::XMFLOAT3& center)
{
	this->center = center;
}

void SphereCollider::setCenter(DirectX::XMVECTOR& center)
{
	DirectX::XMStoreFloat3(&this->center, center);
}

void SphereCollider::setRadius(float radius)
{
	this->radius = radius;
}

IntersectionData SphereCollider::isIntersected(const Collider& other)
{
	switch (other.getType())
	{
	case Collider::Plane:
		assert("Sphere to Plane intersection not implemented");
		break;
	case Collider::Sphere:
		return isIntersected_Implementation((SphereCollider&)other);
		break;
	case Collider::Cube:
		assert("Sphere to Cube intersection not implemented");
		break;
	default:
		assert("default not allowed");
		break;
	}
}

IntersectionData SphereCollider::isIntersected_Implementation(const SphereCollider& other) const
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
