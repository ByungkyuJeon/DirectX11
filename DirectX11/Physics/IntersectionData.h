#pragma once

class IntersectionData
{
public:
	IntersectionData() = default;
	IntersectionData(bool intersectionState, float distance);

	bool getIntersectionState();
	float getIntersectionDistance();

	void setIntersectionState(bool intersectionState);
	void setIntersectionDistance(float distance);

private:
	bool intersectionState;
	float distance;
};