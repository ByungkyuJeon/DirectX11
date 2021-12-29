#pragma once
#include<DirectXMath.h>

struct alignas(16) CB_VS_vertexshader
{
public:
	CB_VS_vertexshader() : xOffset{ 0.0f }, yOffset{ 0.0f }{}

	float xOffset;
	float yOffset;
};