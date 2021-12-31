#include "Model.h"

bool Model::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexShader)
{
	this->device = device;
	this->deviceContext = deviceContext;
	this->texture = texture;
	this->cb_vs_vertexShader = &cb_vs_vertexShader;
	
	try
	{
		if (!this->LoadModel(filePath))
		{
			return false;
		}
	}
	catch (COMException& exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}

	this->pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->posVector = XMLoadFloat3(&this->pos);
	this->rot = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotVector = XMLoadFloat3(&this->rot);

	this->UpdateWorldMatrix();

	return true;
}

void Model::SetTexture(ID3D11ShaderResourceView* texture)
{
	this->texture = texture;
}

void Model::Draw(const DirectX::XMMATRIX& viewProjectionMatrix)
{
	this->cb_vs_vertexShader->data.mat = DirectX::XMMatrixMultiply(this->worldMatrix, viewProjectionMatrix);
	this->cb_vs_vertexShader->data.mat = DirectX::XMMatrixTranspose(this->cb_vs_vertexShader->data.mat);
	this->cb_vs_vertexShader->ApplyChanges();

	this->deviceContext->VSSetConstantBuffers(0, 1, this->cb_vs_vertexShader->GetAddressOf());
	this->deviceContext->PSSetShaderResources(0, 1, &this->texture);

	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw();
	}
}

void Model::UpdateWorldMatrix()
{
	this->worldMatrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z), DirectX::XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z));
	DirectX::XMMATRIX vecRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
	this->vec_forward = DirectX::XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	this->vec_right = DirectX::XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
	this->vec_left = DirectX::XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	this->vec_backward = DirectX::XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	this->vec_up = DirectX::XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, vecRotationMatrix);
	this->vec_down = DirectX::XMVector3TransformCoord(this->DEFAULT_DOWN_VECTOR, vecRotationMatrix);
}

bool Model::LoadModel(const std::string& filePath)
{
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (pScene == nullptr)
	{
		return false;
	}

	this->ProcessNode(pScene->mRootNode, pScene);

	return true;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.emplace_back(this->ProcessMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

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

	return Mesh(this->device, this->deviceContext, vertices, indices);
}

const DirectX::XMVECTOR& Model::GetPositionVector() const
{
	return this->posVector;
}

const DirectX::XMFLOAT3& Model::GetPositionFloat3() const
{
	return this->pos;
}

const DirectX::XMVECTOR& Model::GetRotationVector() const
{
	return this->rotVector;
}

const DirectX::XMFLOAT3& Model::GetRotationFloat3() const
{
	return this->rot;
}

void Model::SetPosition(const DirectX::XMVECTOR& pos)
{
	DirectX::XMStoreFloat3(&this->pos, pos);
	this->posVector = pos;
	this->UpdateWorldMatrix();
}

void Model::SetPosition(const DirectX::XMFLOAT3& pos)
{
	this->pos = pos;
	this->posVector = DirectX::XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void Model::SetPosition(float x, float y, float z)
{
	this->pos = DirectX::XMFLOAT3(x, y, z);
	this->posVector = DirectX::XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void Model::AdjustPosition(const DirectX::XMVECTOR& pos)
{
	this->posVector = DirectX::XMVectorAdd(this->posVector, pos);
	this->UpdateWorldMatrix();
}

void Model::AdjustPosition(const DirectX::XMFLOAT3& pos)
{
	this->pos.x += pos.x;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
	this->posVector = DirectX::XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void Model::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = DirectX::XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void Model::SetRotation(const DirectX::XMVECTOR& rot)
{
	DirectX::XMStoreFloat3(&this->rot, rot);
	this->rotVector = rot;
	this->UpdateWorldMatrix();
}

void Model::SetRotation(const DirectX::XMFLOAT3& rot)
{
	this->rot = rot;
	this->rotVector = DirectX::XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void Model::SetRotation(float x, float y, float z)
{
	this->rot = DirectX::XMFLOAT3(x, y, z);
	this->rotVector = DirectX::XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void Model::AdjustRotation(const DirectX::XMVECTOR& rot)
{
	this->rotVector = DirectX::XMVectorAdd(this->rotVector, rot);
	this->UpdateWorldMatrix();
}

void Model::AdjustRotation(const DirectX::XMFLOAT3& rot)
{
	this->rot.x += rot.x;
	this->rot.y += rot.y;
	this->rot.z += rot.z;
	this->rotVector = DirectX::XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void Model::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = DirectX::XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void Model::SetLookAtPos(DirectX::XMFLOAT3 lookAtPos)
{
	// lookat 위치와 카메라 위치 같음 체크
	if (lookAtPos.x == this->pos.x && lookAtPos.y == this->pos.y && lookAtPos.z == this->pos.z)
	{
		return;
	}

	lookAtPos.x = this->pos.x - lookAtPos.x;
	lookAtPos.y = this->pos.y - lookAtPos.y;
	lookAtPos.z = this->pos.z - lookAtPos.z;

	float pitch = 0.0f;
	if (lookAtPos.y != 0.0f)
	{
		const float distance = sqrt(pow(lookAtPos.x, 2) + pow(lookAtPos.z, 2));
		pitch = atan(lookAtPos.y / distance);
	}

	float yaw = 0.0f;
	if (lookAtPos.x != 0.0f)
	{
		yaw = atan(lookAtPos.x / lookAtPos.z);
	}
	if (lookAtPos.z > 0)
	{
		yaw += DirectX::XM_PI;
	}

	this->SetRotation(pitch, yaw, 0.0f);
}

const DirectX::XMVECTOR& Model::GetForwardVector()
{
	return this->vec_forward;
}

const DirectX::XMVECTOR& Model::GetRightVector()
{
	return this->vec_right;
}

const DirectX::XMVECTOR& Model::GetLeftVector()
{
	return this->vec_left;
}

const DirectX::XMVECTOR& Model::GetBackwardVector()
{
	return this->vec_backward;
}

const DirectX::XMVECTOR& Model::GetUpVector()
{
	return this->vec_up;
}

const DirectX::XMVECTOR& Model::GetDownVector()
{
	return this->vec_down;
}
