#pragma once

#include "Collider.h"

class AxisAlignedBoundingBox : public Collider
{
public:
	AxisAlignedBoundingBox() = default;
	AxisAlignedBoundingBox(DirectX::XMVECTOR& minExtent, DirectX::XMVECTOR& maxExtent);

	const DirectX::XMVECTOR& getMinExtent();
	const DirectX::XMVECTOR& getMaxExtent();

	void setMinExtent(const DirectX::XMVECTOR& extent);
	void setMaxExtent(const DirectX::XMVECTOR& extent);

	IntersectionData isIntersected(const Collider& other) const;

private:
	DirectX::XMVECTOR minExtent;
	DirectX::XMVECTOR maxExtent;
};