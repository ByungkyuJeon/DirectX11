#pragma once

#include <string>
#include <d3d11.h>
#include <wrl/client.h>

class TVertexShader
{
public:
	TVertexShader() : m_Shader{ NULL }, m_ShaderBuffer{ NULL }, m_InputLayout{ NULL } {}

	bool Initialize(const std::wstring& InShaderPath, D3D11_INPUT_ELEMENT_DESC * InLayoutDesc, UINT InNumElements);
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetShader();
	Microsoft::WRL::ComPtr<ID3D10Blob> GetShaderBuffer();
	Microsoft::WRL::ComPtr<ID3D11InputLayout> GetInputLayout();

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_Shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_ShaderBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
};

class TPixelShader
{
public:
	TPixelShader() :m_Shader{ NULL }, m_ShaderBuffer{ NULL } {}

	bool Initialize(const std::wstring& InShaderPath);
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetShader();
	Microsoft::WRL::ComPtr<ID3D10Blob> GetShaderBuffer();

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_Shader;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_ShaderBuffer;
};
