#include "ModelManager.h"
#include "../StringHelper.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <DirectXMath.h>

bool ModelManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->device = device;
	this->deviceContext = deviceContext;
	return true;
}

std::shared_ptr<Model> ModelManager::Instanciate(const std::string& filePath)
{
	try
	{
		std::shared_ptr<Model> ret = std::make_shared<Model>();
		ret->setDirectory(StringHelper::GetdirectoryFromPath(filePath));

		Assimp::Importer importer;

		const aiScene* pScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

		if (pScene == nullptr)
		{
			return nullptr;
		}

		this->ProcessNode(ret, pScene->mRootNode, pScene, DirectX::XMMatrixIdentity());
		return ret;
	}
	catch (COMException& exception)
	{
		ErrorLogger::Log(exception);
		return nullptr;
	}
}

void ModelManager::ProcessNode(std::shared_ptr<Model>& model, aiNode* node, const aiScene* scene, const DirectX::XMMATRIX& parentTransformMatrix)
{
	DirectX::XMMATRIX nodeTansformMatrix = DirectX::XMMatrixTranspose(DirectX::XMMATRIX(&node->mTransformation.a1)) * parentTransformMatrix;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->ProcessMesh(model, mesh, scene, nodeTansformMatrix);
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(model, node->mChildren[i], scene, nodeTansformMatrix);
	}
}

void ModelManager::ProcessMesh(std::shared_ptr<Model>& model, aiMesh* mesh, const aiScene* scene, const DirectX::XMMATRIX& parentTransformMatrix)
{
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.texCoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texCoord.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.emplace_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			indices.emplace_back(face.mIndices[j]);
		}
	}

	std::vector<Texture> textures;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> diffuseTextures = LoadMaterialTextures(model, material, aiTextureType::aiTextureType_DIFFUSE, scene);
	textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

	Mesh ret;
	VertexBuffer<Vertex> vertexBuffer;
	HRESULT hr = vertexBuffer.Initialize(device, vertices.data(), vertices.size());
	COM_ERROR_IF_FAILED(hr, "vertexBuffer Initialzation failed.");

	IndexBuffer indexBuffer;
	hr = indexBuffer.Initialize(device, indices.data(), indices.size());
	COM_ERROR_IF_FAILED(hr, "indexBuffer Initialzation failed.");

	model->addMesh(Mesh(vertexBuffer, indexBuffer, textures, parentTransformMatrix));
}

std::vector<Texture> ModelManager::LoadMaterialTextures(std::shared_ptr<Model>& model, aiMaterial* pMaterial, aiTextureType type, const aiScene* pScene)
{
	std::vector<Texture> materialTextures;
	TextureStorageType storeType = TextureStorageType::Invalid;
	unsigned int textureCount = pMaterial->GetTextureCount(type);

	if (textureCount == 0)
	{
		storeType = TextureStorageType::None;
		aiColor3D aiColor(0.0f, 0.0f, 0.0f);
		switch (type)
		{
		case aiTextureType_DIFFUSE:
			pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
			if (aiColor.IsBlack())
			{
				materialTextures.emplace_back(Texture(this->device, Colors::UnloadedTextureColor, type));
				return materialTextures;
			}
			materialTextures.emplace_back(Texture(this->device, Color(aiColor.r * 255, aiColor.g * 255, aiColor.b * 255), type));
			return materialTextures;
			break;
		}
	}
	else
	{
		for (UINT i = 0; i < textureCount; i++)
		{
			aiString path;
			pMaterial->GetTexture(type, i, &path);
			TextureStorageType storageType = DeterminTextureStorageType(pScene, pMaterial, i, type);
			switch (storeType)
			{
			case TextureStorageType::EmbeddedIndexCompressed:
			{
				int index = GetTextureIndex(&path);
				Texture embeddedIndexedTexture(this->device, reinterpret_cast<uint8_t*>(pScene->mTextures[index]->pcData), pScene->mTextures[index]->mWidth, type);
				materialTextures.emplace_back(embeddedIndexedTexture);
				break;
			}

			case TextureStorageType::EmbeddedCompressed:
			{
				const aiTexture* pTexture = pScene->GetEmbeddedTexture(path.C_Str());
				Texture embeddedTexture(this->device, reinterpret_cast<uint8_t*>(pTexture->pcData), pTexture->mWidth, type);
				materialTextures.emplace_back(embeddedTexture);
				break;
			}
			case TextureStorageType::Disk:
			{
				std::string fileName = model->getDirectory() + '\\' + path.C_Str();
				Texture diskTexture(this->device, fileName, type);
				materialTextures.emplace_back(diskTexture);
				break;
			}
			}
		}
	}

	if (materialTextures.size() == 0)
	{
		materialTextures.emplace_back(Texture(this->device, Colors::UnhandledTextureColor, type));
	}
	return materialTextures;
}

TextureStorageType ModelManager::DeterminTextureStorageType(const aiScene* pScene, aiMaterial* pMat, unsigned int index, aiTextureType type)
{
	if (pMat->GetTextureCount(type) == 0)
	{
		return TextureStorageType::None;
	}

	aiString path;
	pMat->GetTexture(type, index, &path);
	std::string texturePath = path.C_Str();
	if (texturePath[0] == '*')
	{
		if (pScene->mTextures[0]->mHeight == 0)
		{
			return TextureStorageType::EmbeddedIndexCompressed;
		}
		else
		{
			return TextureStorageType::EmbeddedIndexNonCompressed;
		}
	}

	if (auto pTex = pScene->GetEmbeddedTexture(texturePath.c_str()))
	{
		if (pTex->mHeight == 0)
		{
			return TextureStorageType::EmbeddedCompressed;
		}
		else
		{
			return TextureStorageType::EmbeddedNoncompressed;
		}
	}

	if (texturePath.find('.') != std::string::npos)
	{
		return TextureStorageType::Disk;
	}

	return TextureStorageType::None;
}

int ModelManager::GetTextureIndex(aiString* pStr)
{
	return atoi(&pStr->C_Str()[1]);
}
