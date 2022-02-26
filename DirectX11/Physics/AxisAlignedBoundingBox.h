#pragma once

#include "IntersectionData.h"
#include <DirectXMath.h>

class AxisAlignedBoundingBox
{
public:
	AxisAlignedBoundingBox() = default;
	AxisAlignedBoundingBox(DirectX::XMVECTOR& minExtent, DirectX::XMVECTOR& maxExtent);

	const DirectX::XMVECTOR& getMinExtent();
	const DirectX::XMVECTOR& getMaxExtent();

	void setMinExtent(const DirectX::XMVECTOR& extent);
	void setMaxExtent(const DirectX::XMVECTOR& extent);

	IntersectionData isIntersected(const AxisAlignedBoundingBox& other) const;

private:
	DirectX::XMVECTOR minExtent;
	DirectX::XMVECTOR maxExtent;
};