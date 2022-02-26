#include "AxisAlignedBoundingBox.h"

AxisAlignedBoundingBox::AxisAlignedBoundingBox(DirectX::XMVECTOR& minExtent, DirectX::XMVECTOR& maxExtent) :
	minExtent{ minExtent }, maxExtent { maxExtent }
{
}

const DirectX::XMVECTOR& AxisAlignedBoundingBox::getMinExtent()
{
	return minExtent;
}

const DirectX::XMVECTOR& AxisAlignedBoundingBox::getMaxExtent()
{
	return maxExtent;
}

void AxisAlignedBoundingBox::setMinExtent(const DirectX::XMVECTOR& extent)
{
	this->minExtent = extent;
}

void AxisAlignedBoundingBox::setMaxExtent(const DirectX::XMVECTOR& extent)
{
	this->maxExtent = extent;
}

IntersectionData AxisAlignedBoundingBox::isIntersected(const Collider& other) const
{
	return IntersectionData();
}
