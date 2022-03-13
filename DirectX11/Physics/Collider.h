#pragma once
#include "IntersectionData.h"
#include "../CoordinateSystem/Transform.h"
#include <memory>

class Collider
{
public:
	enum Type
	{
		Plane,
		Sphere,
		Cube
	};

	virtual IntersectionData isIntersected(Collider* other) = 0;

	void setType(Type type);

	Type getType() const;

protected:
	Type mType;
};