#pragma once

class TEngineMath
{
public:
	inline static float max(float fir, float sec, float thir)
	{
		float mid = fir > sec ? fir : sec;
		return mid > thir ? mid : thir;
	}
};