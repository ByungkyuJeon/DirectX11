#pragma once

#include "IntersectionData.h"
#include <DirectXMath.h>

class Collider
{
	virtual IntersectionData isIntersected(const Collider& other) const = 0;
};