#include "Mesh.h"

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, std::vector<Texture>& textures, const DirectX::XMMATRIX& transformMatrix)
{
	this->transformMatrix = transformMatrix;
	this->deviceContext = deviceContext;
	this->textures = textures;
	
	HRESULT hr = this->vertexBuffer.Initialize(device, vertices.data(), vertices.size());
	COM_ERROR_IF_FAILED(hr, "vertexBuffer Initialzation failed.");

	hr = this->indexBuffer.Initialize(device, indices.data(), indices.size());
	COM_ERROR_IF_FAILED(hr, "indexBuffer Initialzation failed.");
}

Mesh::Mesh(const Mesh& mesh)
{
	this->deviceContext = mesh.deviceContext;
	this->indexBuffer = mesh.indexBuffer;
	this->vertexBuffer = mesh.vertexBuffer;
	this->textures = mesh.textures;
	this->transformMatrix = mesh.transformMatrix;
}

void Mesh::Draw()
{
	UINT offset = 0;

	for (int idx = 0; idx < this->textures.size(); idx++)
	{
		if (this->textures[idx].GetType() == aiTextureType::aiTextureType_DIFFUSE)
		{
			this->deviceContext->PSSetShaderResources(0, 1, textures[idx].GetTextureResourceViewAddress());
			break;
		}
	}

	this->deviceContext->IASetVertexBuffers(0, 1, this->vertexBuffer.GetAddressOf(), this->vertexBuffer.StridePtr(), &offset);
	this->deviceContext->IASetIndexBuffer(this->indexBuffer.Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	this->deviceContext->DrawIndexed(this->indexBuffer.IndexCount(), 0, 0);
}

const DirectX::XMMATRIX& Mesh::GetTransformMatrix()
{
	return this->transformMatrix;
}
