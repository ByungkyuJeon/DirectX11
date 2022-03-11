#pragma once

#include "../Graphics/Model.h"
#include <memory>

class ModelManager
{
public:
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	std::shared_ptr<Model> Instanciate(const std::string& filePath);

private:
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;


	void ProcessNode(std::shared_ptr<Model>& model, aiNode* node, const aiScene* scene, const DirectX::XMMATRIX& parentTransformMatrix);
	void ProcessMesh(std::shared_ptr<Model>& model, aiMesh* mesh, const aiScene* scene, const DirectX::XMMATRIX& parentTransformMatrix);
	std::vector<Texture> LoadMaterialTextures(std::shared_ptr<Model>& model, aiMaterial* pMaterial, aiTextureType type, const aiScene* pScene);
	TextureStorageType DeterminTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType type);
	int GetTextureIndex(aiString* pStr);
};