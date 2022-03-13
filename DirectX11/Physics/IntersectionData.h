#pragma once

#include <DirectXMath.h>

class IntersectionData
{
public:
	IntersectionData() = default;
	IntersectionData(bool intersectionState, float distance);

	bool getIntersectionState();
	float getIntersectionDistance();
	const DirectX::XMFLOAT3 getLtoRNormed() const;

	void setLtoRNormed(const DirectX::XMFLOAT3& ltoRNormed);
	void setIntersectionState(bool intersectionState);
	void setIntersectionDistance(float distance);

private:
	bool intersectionState;
	float distance;
	DirectX::XMFLOAT3 mLtoRNormed;
};