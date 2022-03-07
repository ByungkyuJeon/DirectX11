#include "PlaneCollider.h"

PlaneCollider::PlaneCollider(const DirectX::XMFLOAT3& normal, float distance) :
	normal{ normal }, distance{ distance }
{
}

PlaneCollider::PlaneCollider(const DirectX::XMVECTOR& normal, float distance) :
	distance{ distance }
{
	DirectX::XMStoreFloat3(&this->normal, normal);
}

PlaneCollider PlaneCollider::normalized() const
{
	float magnitude = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMLoadFloat3(&this->normal)));
	return PlaneCollider(
		DirectX::XMVectorDivide(
			DirectX::XMLoadFloat3(&this->normal),
			DirectX::XMVectorReplicate(magnitude)
		),
		this->distance / magnitude
	);
}

const DirectX::XMFLOAT3& PlaneCollider::getNormal() const
{
	return this->normal;
}

float PlaneCollider::getDistance() const
{
	return this->distance;
}

void PlaneCollider::setNormal(const DirectX::XMFLOAT3& normal)
{
	this->normal = normal;
}

void PlaneCollider::setNormal(const DirectX::XMVECTOR& normal)
{
	DirectX::XMStoreFloat3(&this->normal, normal);
}

void PlaneCollider::setDistance(float distance)
{
	this->distance = distance;
}

IntersectionData PlaneCollider::isIntersected(const SphereCollider& other) const
{
	float distance = fabsf(this->distance 
		+ DirectX::XMVectorGetX(
		DirectX::XMVector3Dot(
			DirectX::XMLoadFloat3(&this->normal),
			DirectX::XMLoadFloat3(&other.getCenter())
			)
	));
	return IntersectionData(distance <= other.getRadius(), distance - other.getRadius());
}

IntersectionData PlaneCollider::isIntersected(const AxisAlignedBoundingBox& other) const
{

	return IntersectionData();
}
