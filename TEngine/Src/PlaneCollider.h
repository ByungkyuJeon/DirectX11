#pragma once

#include <DirectXMath.h>
#include "IntersectionData.h"
#include "Collider.h"
#include "AxisAlignedBoundingBox.h"

class SphereCollider;

class PlaneCollider : public Collider
{
public:
	PlaneCollider() = default;
	PlaneCollider(const DirectX::XMFLOAT3& normal, float distance);
	PlaneCollider(const DirectX::XMVECTOR& normal, float distance);

	void setTransform(float x, float y, float z) override;

	PlaneCollider normalized() const;

	const DirectX::XMFLOAT3& getNormal() const;
	float getDistance() const;

	void setNormal(const DirectX::XMFLOAT3& normal);
	void setNormal(const DirectX::XMVECTOR& normal);
	void setDistance(float distance);

	IntersectionData isIntersected(Collider* other) override;

	IntersectionData isIntersected_Implementation(SphereCollider* other) const;

private:
	DirectX::XMFLOAT3	normal;
	float				distance;
};