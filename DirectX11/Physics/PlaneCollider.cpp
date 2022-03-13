#include "PlaneCollider.h"
#include "SphereCollider.h"

PlaneCollider::PlaneCollider(const DirectX::XMFLOAT3& normal, float distance) :
	normal{ normal }, distance{ distance }
{
	this->setType(Collider::Type::Plane);
}

PlaneCollider::PlaneCollider(const DirectX::XMVECTOR& normal, float distance) :
	distance{ distance }
{
	DirectX::XMStoreFloat3(&this->normal, normal);
	this->setType(Collider::Type::Plane);
}

void PlaneCollider::setTransform(float x, float y, float z)
{
	this->distance = y;
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

IntersectionData PlaneCollider::isIntersected(Collider* other)
{
	switch (other->getType())
	{
	case Collider::Plane:
		assert("default not allowed");
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

IntersectionData PlaneCollider::isIntersected_Implementation(SphereCollider* other) const
{
	IntersectionData ret;
	float distance = fabsf(fabsf(this->distance)
		+ DirectX::XMVectorGetX(
			DirectX::XMVector3Dot(
				DirectX::XMLoadFloat3(&this->normal),
				DirectX::XMLoadFloat3(&other->getCenter())
			)
		));

	ret.setIntersectionState(distance <= other->getRadius());
	ret.setIntersectionDistance(distance - other->getRadius());
	ret.setLtoRNormed(this->normal.x, this->normal.y, this->normal.z);

	return ret;
}
