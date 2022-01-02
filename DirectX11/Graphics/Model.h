#pragma once
#include "Mesh.h"

class Model
{
public:
	bool Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexShader);
	void Draw(const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewProjectionMatrix);

private:

	std::vector<Mesh> meshes;
	bool LoadModel(const std::string& filePath);
	void ProcessNode(aiNode* node, const aiScene* scene, const DirectX::XMMATRIX& parentTransformMatrix);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, const DirectX::XMMATRIX& parentTransformMatrix);
	TextureStorageType DeterminTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType type);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType type, const aiScene* pScene);
	int GetTextureIndex(aiString* pStr);

	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	ConstantBuffer<CB_VS_vertexshader>* cb_vs_vertexShader = nullptr;
	std::string directory = "";

};