#pragma once

#include <DirectXMath.h>
#include "IntersectionData.h"
#include "BoundingSphere.h"
#include "AxisAlignedBoundingBox.h"

class PlaneCollider
{
public:
	PlaneCollider() = default;
	PlaneCollider(const DirectX::XMFLOAT3& normal, float distance);
	PlaneCollider(const DirectX::XMVECTOR& normal, float distance);

	PlaneCollider normalized() const;

	const DirectX::XMFLOAT3& getNormal() const;
	float getDistance() const;

	void setNormal(const DirectX::XMFLOAT3& normal);
	void setNormal(const DirectX::XMVECTOR& normal);
	void setDistance(float distance);

	IntersectionData isIntersected(const BoundingSphere& other) const;
	IntersectionData isIntersected(const AxisAlignedBoundingBox& other) const;

private:
	DirectX::XMFLOAT3	normal;
	float				distance;
};