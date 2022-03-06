#pragma once
#include "IntersectionData.h"

class Collider
{
public:
	enum Type
	{
		Plane,
		Sphere,
		Cube
	};

	virtual IntersectionData isIntersected(const Collider& other) = 0;

	void setType(Type type);

	Type getType() const;

protected:
	Type mType;
};