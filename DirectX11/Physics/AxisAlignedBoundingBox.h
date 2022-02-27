#pragma once

#include "IntersectionData.h"
#include <DirectXMath.h>

class AxisAlignedBoundingBox
{
public:
	AxisAlignedBoundingBox() = default;
	AxisAlignedBoundingBox(DirectX::XMVECTOR& minExtent, DirectX::XMVECTOR& maxExtent);
	AxisAlignedBoundingBox(DirectX::XMFLOAT3& minExtent, DirectX::XMFLOAT3& maxExtent);

	const DirectX::XMFLOAT3& getMinExtent() const;
	const DirectX::XMFLOAT3& getMaxExtent() const;

	void setMinExtent(const DirectX::XMVECTOR& extent);
	void setMaxExtent(const DirectX::XMVECTOR& extent);
	void setMinExtent(const DirectX::XMFLOAT3& extent);
	void setMaxExtent(const DirectX::XMFLOAT3& extent);

	IntersectionData isIntersected(const AxisAlignedBoundingBox& other) const;

private:
	// XMVECTOR doesn't automatically aligned as 16 bytes in class
	DirectX::XMFLOAT3 minExtent;
	DirectX::XMFLOAT3 maxExtent;
};