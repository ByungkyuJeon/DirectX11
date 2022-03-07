#pragma once
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>
#include "Texture.h"

class Mesh
{
public:
	Mesh() = default;
	Mesh(const VertexBuffer<Vertex>& vertexBuffer, const IndexBuffer& indexBuffer, const std::vector<Texture>& textures, const DirectX::XMMATRIX& transformMatrix);
	Mesh(const Mesh& mesh);

	void setVertexBuffer(const VertexBuffer<Vertex>& vertexBuffer);
	void setIndexBuffer(const IndexBuffer& indexBuffer);

	const VertexBuffer<Vertex>& getVertexBuffer();
	const IndexBuffer& getIndxBuffer();

	const DirectX::XMMATRIX& GetTransformMatrix();
	const std::vector<Texture>& getTextures() const;
	Texture& getTextureAt(size_t idx);

private:
	VertexBuffer<Vertex> mVertexBuffer;
	IndexBuffer mIndexBuffer;
	std::vector<Texture> textures;
	DirectX::XMMATRIX transformMatrix;
};