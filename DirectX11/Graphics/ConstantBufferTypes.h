#pragma once
#include<DirectXMath.h>

struct alignas(16) CB_VS_vertexshader
{
public:
	CB_VS_vertexshader() : wvpMatrix{ DirectX::XMMatrixIdentity() }, worldMatrix{ DirectX::XMMatrixIdentity() }{}

	DirectX::XMMATRIX wvpMatrix;
	DirectX::XMMATRIX worldMatrix;
};

struct alignas(16) CB_PS_light
{
	DirectX::XMFLOAT3 ambientLightColor;
	float ambientLightStrength;
};