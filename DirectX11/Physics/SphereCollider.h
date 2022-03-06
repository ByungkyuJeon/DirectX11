#pragma once

#include "Collider.h"
#include "IntersectionData.h"
#include <DirectXMath.h>

class SphereCollider : public Collider
{
public:
	SphereCollider() = default;
	SphereCollider(DirectX::XMVECTOR& center, float radius);
	SphereCollider(DirectX::XMFLOAT3& center, float radius);

	const DirectX::XMFLOAT3& getCenter() const;
	float getRadius() const;

	void setCenter(DirectX::XMFLOAT3& center);
	void setCenter(DirectX::XMVECTOR& center);
	void setRadius(float radius);

	IntersectionData isIntersected(const Collider& other) override;

	IntersectionData isIntersected_Implementation(const SphereCollider& other) const;

private:
	DirectX::XMFLOAT3	center;
	float				radius;
};