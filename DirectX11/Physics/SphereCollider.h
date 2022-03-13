#pragma once

#include "Collider.h"
#include "IntersectionData.h"
#include <DirectXMath.h>

class PlaneCollider;

class SphereCollider : public Collider
{
public:
	SphereCollider() = default;
	SphereCollider(const DirectX::XMVECTOR& center, float radius);
	SphereCollider(const DirectX::XMFLOAT3& center, float radius);

	void setTransform(float x, float y, float z) override;
	const DirectX::XMFLOAT3& getCenter() const;
	float getRadius() const;

	void setCenter(DirectX::XMFLOAT3& center);
	void setCenter(DirectX::XMVECTOR& center);
	void setRadius(float radius);

	IntersectionData isIntersected(Collider* other) override;

	IntersectionData isIntersected_Implementation(SphereCollider* other) const;
	IntersectionData isIntersected_Implementation(PlaneCollider* other) const;

private:
	DirectX::XMFLOAT3	mCenter;
	float				mRadius;
};