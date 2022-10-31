#pragma once

#include "TEngineContext.h"
#include "TErrorLogger.h"
#include "ConstantBufferTypes.h"

template<class T>
class ConstantBuffer
{
private:
	ConstantBuffer(ConstantBuffer<T>& rhs);

private:

	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

public:
	ConstantBuffer() {}

	T data;

	ID3D11Buffer* Get() const
	{
		return this->buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const
	{
		return this->buffer.GetAddressOf();
	}

	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		if (buffer.Get() != nullptr)
		{
			buffer.Reset();
		}

		// TODO :: deviceContext 인자 제거 필요
		//this->deviceContext = deviceContext;

		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = static_cast<UINT>(sizeof(T));
		desc.StructureByteStride = 0;

		return gDevice->CreateBuffer(&desc, 0, buffer.GetAddressOf());
	}

	bool ApplyChanges()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = gDeviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(hr))
		{
			PRINT_ERROR("constant buffer mapping failed.");
			return false;
		}
		CopyMemory(mappedResource.pData, &data, sizeof(T));
		gDeviceContext->Unmap(buffer.Get(), 0);
		return true;
	}
};