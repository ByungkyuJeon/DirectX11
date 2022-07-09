#pragma once

#include "../../Core.h"

class TVertexShader
{
public:
	TVertexShader() : m_Shader{ NULL }, m_ShaderBuffer{ NULL }, m_InputLayout{ NULL } {}

	bool Initialize(TWString InShaderPath, D3D11_INPUT_ELEMENT_DESC * InLayoutDesc, UINT InNumElements);
	TComPtr<ID3D11VertexShader> GetShader();
	TComPtr<ID3D10Blob> GetShaderBuffer();
	TComPtr<ID3D11InputLayout> GetInputLayout();

private:
	TComPtr<ID3D11VertexShader> m_Shader;
	TComPtr<ID3D10Blob> m_ShaderBuffer;
	TComPtr<ID3D11InputLayout> m_InputLayout;
};

class TPixelShader
{
public:
	TPixelShader() :m_Shader{ NULL }, m_ShaderBuffer{ NULL } {}

	bool Initialize(TWString InShaderPath);
	TComPtr<ID3D11PixelShader> GetShader();
	TComPtr<ID3D10Blob> GetShaderBuffer();

private:
	TComPtr<ID3D11PixelShader> m_Shader;
	TComPtr<ID3D10Blob> m_ShaderBuffer;
};
