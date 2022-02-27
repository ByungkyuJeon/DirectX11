#pragma once

#include "IntersectionData.h"
#include <DirectXMath.h>

class BoundingSphere
{
public:
	BoundingSphere() = default;
	BoundingSphere(DirectX::XMVECTOR& center, float radius);
	BoundingSphere(DirectX::XMFLOAT3& center, float radius);

	const DirectX::XMFLOAT3& getCenter() const;
	float getRadius() const;

	void setCenter(DirectX::XMFLOAT3& center);
	void setCenter(DirectX::XMVECTOR& center);
	void setRadius(float radius);

	IntersectionData isIntersected(const BoundingSphere& other) const;

private:
	DirectX::XMFLOAT3	center;
	float				radius;
};