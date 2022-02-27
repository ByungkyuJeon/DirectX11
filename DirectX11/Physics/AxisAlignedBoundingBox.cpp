#include "AxisAlignedBoundingBox.h"
#include "../CustomMath.h"

AxisAlignedBoundingBox::AxisAlignedBoundingBox(DirectX::XMVECTOR& minExtent, DirectX::XMVECTOR& maxExtent)
{
	DirectX::XMStoreFloat3(&this->minExtent, minExtent);
	DirectX::XMStoreFloat3(&this->maxExtent, maxExtent);
}

AxisAlignedBoundingBox::AxisAlignedBoundingBox(DirectX::XMFLOAT3& minExtent, DirectX::XMFLOAT3& maxExtent) :
	minExtent{ minExtent }, maxExtent{ maxExtent }
{
}

const DirectX::XMFLOAT3& AxisAlignedBoundingBox::getMinExtent() const
{
	return this->minExtent;
}

const DirectX::XMFLOAT3& AxisAlignedBoundingBox::getMaxExtent() const
{
	return this->maxExtent;
}

void AxisAlignedBoundingBox::setMinExtent(const DirectX::XMVECTOR& extent)
{
	DirectX::XMStoreFloat3(&this->minExtent, extent);
}

void AxisAlignedBoundingBox::setMaxExtent(const DirectX::XMVECTOR& extent)
{
	DirectX::XMStoreFloat3(&this->maxExtent, extent);
}

void AxisAlignedBoundingBox::setMinExtent(const DirectX::XMFLOAT3& extent)
{
	this->minExtent = extent;
}

void AxisAlignedBoundingBox::setMaxExtent(const DirectX::XMFLOAT3& extent)
{
	this->maxExtent = extent;
}

IntersectionData AxisAlignedBoundingBox::isIntersected(const AxisAlignedBoundingBox& other) const
{
	DirectX::XMVECTOR lhs[2]{ DirectX::XMLoadFloat3(&this->minExtent), DirectX::XMLoadFloat3(&this->maxExtent) };
	DirectX::XMVECTOR rhs[2]{ DirectX::XMLoadFloat3(&other.minExtent), DirectX::XMLoadFloat3(&other.maxExtent) };

	DirectX::XMFLOAT3 lhsDistance;
	DirectX::XMFLOAT3 rhsDistance;

	DirectX::XMStoreFloat3(&lhsDistance, DirectX::XMVectorSubtract(rhs[0], lhs[1]));
	DirectX::XMStoreFloat3(&rhsDistance, DirectX::XMVectorSubtract(lhs[0], rhs[1]));

	DirectX::XMFLOAT3 Distances
	{ 
		lhsDistance.x > rhsDistance.x ? lhsDistance.x : rhsDistance.x,
		lhsDistance.y > rhsDistance.y ? lhsDistance.y : rhsDistance.y,
		lhsDistance.z > rhsDistance.z ? lhsDistance.z : rhsDistance.z
	};

	float max = CustomMath::max(Distances.x, Distances.y, Distances.z);

	return IntersectionData{ max < 0, max };
}
