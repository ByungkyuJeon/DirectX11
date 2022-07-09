#include "Shaders.h"


bool TVertexShader::Initialize(TWString InShaderPath, D3D11_INPUT_ELEMENT_DESC* InLayoutDesc, UINT InNumElements)
{
	HRESULT hResult;

	hResult = D3DReadFileToBlob(InShaderPath.c_str(), m_ShaderBuffer.GetAddressOf());
	if (FAILED(hResult))
	{
		PRINT_ERROR("D3DReadFileToBlob from vertex shader failed.");
		return false;
	}

	hResult = GDevice->CreateVertexShader(m_ShaderBuffer->GetBufferPointer(), m_ShaderBuffer->GetBufferSize(), NULL, m_Shader.GetAddressOf());
	if (FAILED(hResult))
	{
		PRINT_ERROR("CreateVertexShader from vertex shader failed.");
		return false;
	}

	hResult = GDevice->CreateInputLayout(InLayoutDesc, InNumElements, m_ShaderBuffer->GetBufferPointer(), m_ShaderBuffer->GetBufferSize(), m_InputLayout.GetAddressOf());
	if (FAILED(hResult))
	{
		PRINT_ERROR("CreateInputLayout from vertex shader failed.");
		return false;
	}

	return true;
}

TComPtr<ID3D11VertexShader> TVertexShader::GetShader()
{
	return m_Shader;
}

TComPtr<ID3D10Blob> TVertexShader::GetShaderBuffer()
{
	return m_ShaderBuffer;
}

TComPtr<ID3D11InputLayout> TVertexShader::GetInputLayout()
{
	return m_InputLayout;
}

bool TPixelShader::Initialize(TWString InShaderPath)
{
	HRESULT hResult;

	hResult = D3DReadFileToBlob(InShaderPath.c_str(), m_ShaderBuffer.GetAddressOf());
	if (FAILED(hResult))
	{
		PRINT_ERROR("D3DreadFileToBlob from pixel shader failed.");
		return false;
	}

	hResult = GDevice->CreatePixelShader(m_ShaderBuffer->GetBufferPointer(), m_ShaderBuffer->GetBufferSize(), NULL, m_Shader.GetAddressOf());
	if (FAILED(hResult))
	{
		PRINT_ERROR("CreatePixelShader from pixel shader failed.");
		return false;
	}

	return true;
}

TComPtr<ID3D11PixelShader> TPixelShader::GetShader()
{
	return m_Shader;
}

TComPtr<ID3D10Blob> TPixelShader::GetShaderBuffer()
{
	return m_ShaderBuffer;
}
