#include "Mesh.h"

Mesh::Mesh(const VertexBuffer<Vertex>& vertexBuffer, const IndexBuffer& indexBuffer, const std::vector<Texture>& textures, const DirectX::XMMATRIX& transformMatrix)
{
	this->transformMatrix = transformMatrix;
	this->textures = textures;
	this->mVertexBuffer = vertexBuffer;
	this->mIndexBuffer = indexBuffer;
}

Mesh::Mesh(const Mesh& mesh)
{
	this->mIndexBuffer = mesh.mIndexBuffer;
	this->mVertexBuffer = mesh.mVertexBuffer;
	this->textures = mesh.textures;
	this->transformMatrix = mesh.transformMatrix;
}

void Mesh::setVertexBuffer(const VertexBuffer<Vertex>& vertexBuffer)
{
	this->mVertexBuffer = vertexBuffer;
}

void Mesh::setIndexBuffer(const IndexBuffer& indexBuffer)
{
	this->mIndexBuffer = indexBuffer;
}

const VertexBuffer<Vertex>& Mesh::getVertexBuffer()
{
	return this->mVertexBuffer;
}

const IndexBuffer& Mesh::getIndxBuffer()
{
	return this->mIndexBuffer;
}

const DirectX::XMMATRIX& Mesh::GetTransformMatrix()
{
	return this->transformMatrix;
}

const std::vector<Texture>& Mesh::getTextures() const
{
	return this->textures;
}

Texture& Mesh::getTextureAt(size_t idx)
{
	return this->textures[idx];
}
