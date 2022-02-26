#pragma once

#include "Collider.h"

class BoundingSphere : public Collider
{
public:
	BoundingSphere() = default;
	BoundingSphere(DirectX::XMVECTOR& center, float radius) : center{ center }, radius{ radius }{}

	const DirectX::XMVECTOR& getCenter();
	float getRadius();

	void setCenter(DirectX::XMVECTOR& center);
	void setRadius(float radius);

	IntersectionData isIntersected(const BoundingSphere& other) const;

private:
	DirectX::XMVECTOR	center;
	float				radius;
};