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
		Box
	};

	virtual IntersectionData isIntersected(Collider* other) = 0;
	virtual void setTransform(float x, float y, float z);

	void setType(Type type);

	Type getType() const;

protected:
	Type mType;
};