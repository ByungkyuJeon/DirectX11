#include "SphereCollider.h"

SphereCollider::SphereCollider(const DirectX::XMVECTOR& center, float radius) :
	mRadius{ radius }
{
	DirectX::XMStoreFloat3(&this->mCenter, center);
}

SphereCollider::SphereCollider(const DirectX::XMFLOAT3& center, float radius) :
	mCenter{ center }, mRadius{ radius }
{
}

const DirectX::XMFLOAT3& SphereCollider::getCenter() const
{
	return this->mCenter;
}

float SphereCollider::getRadius() const
{
	return this->mRadius;
}

void SphereCollider::setCenter(DirectX::XMFLOAT3& center)
{
	this->mCenter = center;
}

void SphereCollider::setCenter(DirectX::XMVECTOR& center)
{
	DirectX::XMStoreFloat3(&this->mCenter, center);
}

void SphereCollider::setRadius(float radius)
{
	this->mRadius = radius;
}

IntersectionData SphereCollider::isIntersected(Collider* other)
{
	switch (other->getType())
	{
	case Collider::Plane:
		assert("Sphere to Plane intersection not implemented");
		break;
	case Collider::Sphere:
		return isIntersected_Implementation((SphereCollider*)other);
		break;
	case Collider::Box:
		assert("Sphere to Cube intersection not implemented");
		break;
	default:
		assert("default not allowed");
		break;
	}
}

IntersectionData SphereCollider::isIntersected_Implementation(SphereCollider* other) const
{
	IntersectionData ret;
	float dist = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&this->mCenter), DirectX::XMLoadFloat3(&other->mCenter))));
	bool intersectionState = dist <= this->mRadius + other->mRadius;
	ret.setIntersectionState(intersectionState);
	if (intersectionState)
	{
		ret.setIntersectionDistance((this->mRadius + other->mRadius) - dist);
		DirectX::XMFLOAT3 ltoRNormed;
		DirectX::XMStoreFloat3(&ltoRNormed,
		DirectX::XMVector3Normalize(
			DirectX::XMVectorSubtract(
				DirectX::XMLoadFloat3(&other->getCenter()),
				DirectX::XMLoadFloat3(&this->mCenter)
			)));
		ret.setLtoRNormed(ltoRNormed);
	}
	return ret;
}
