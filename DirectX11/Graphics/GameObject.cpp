#include "GameObject.h"

bool GameObject::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* texture, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexShader)
{
	if (!this->model.Initialize(filePath, device, deviceContext, texture, cb_vs_vertexShader))
	{
		return false;
	}

	this->pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->posVector = XMLoadFloat3(&this->pos);
	this->rot = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotVector = XMLoadFloat3(&this->rot);

	this->UpdateWorldMatrix();

	return true;
}

void GameObject::Draw(const DirectX::XMMATRIX& viewProjectionMatrix)
{
	this->model.Draw(this->worldMatrix, viewProjectionMatrix);
}

void GameObject::UpdateWorldMatrix()
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

const DirectX::XMVECTOR& GameObject::GetPositionVector() const
{
	return this->posVector;
}

const DirectX::XMFLOAT3& GameObject::GetPositionFloat3() const
{
	return this->pos;
}

const DirectX::XMVECTOR& GameObject::GetRotationVector() const
{
	return this->rotVector;
}

const DirectX::XMFLOAT3& GameObject::GetRotationFloat3() const
{
	return this->rot;
}

void GameObject::SetPosition(const DirectX::XMVECTOR& pos)
{
	DirectX::XMStoreFloat3(&this->pos, pos);
	this->posVector = pos;
	this->UpdateWorldMatrix();
}

void GameObject::SetPosition(const DirectX::XMFLOAT3& pos)
{
	this->pos = pos;
	this->posVector = DirectX::XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void GameObject::SetPosition(float x, float y, float z)
{
	this->pos = DirectX::XMFLOAT3(x, y, z);
	this->posVector = DirectX::XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void GameObject::AdjustPosition(const DirectX::XMVECTOR& pos)
{
	this->posVector = DirectX::XMVectorAdd(this->posVector, pos);
	this->UpdateWorldMatrix();
}

void GameObject::AdjustPosition(const DirectX::XMFLOAT3& pos)
{
	this->pos.x += pos.x;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
	this->posVector = DirectX::XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void GameObject::AdjustPosition(float x, float y, float z)
{
	this->pos.x += x;
	this->pos.y += y;
	this->pos.z += z;
	this->posVector = DirectX::XMLoadFloat3(&this->pos);
	this->UpdateWorldMatrix();
}

void GameObject::SetRotation(const DirectX::XMVECTOR& rot)
{
	DirectX::XMStoreFloat3(&this->rot, rot);
	this->rotVector = rot;
	this->UpdateWorldMatrix();
}

void GameObject::SetRotation(const DirectX::XMFLOAT3& rot)
{
	this->rot = rot;
	this->rotVector = DirectX::XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void GameObject::SetRotation(float x, float y, float z)
{
	this->rot = DirectX::XMFLOAT3(x, y, z);
	this->rotVector = DirectX::XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void GameObject::AdjustRotation(const DirectX::XMVECTOR& rot)
{
	this->rotVector = DirectX::XMVectorAdd(this->rotVector, rot);
	this->UpdateWorldMatrix();
}

void GameObject::AdjustRotation(const DirectX::XMFLOAT3& rot)
{
	this->rot.x += rot.x;
	this->rot.y += rot.y;
	this->rot.z += rot.z;
	this->rotVector = DirectX::XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void GameObject::AdjustRotation(float x, float y, float z)
{
	this->rot.x += x;
	this->rot.y += y;
	this->rot.z += z;
	this->rotVector = DirectX::XMLoadFloat3(&this->rot);
	this->UpdateWorldMatrix();
}

void GameObject::SetLookAtPos(DirectX::XMFLOAT3 lookAtPos)
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

const DirectX::XMVECTOR& GameObject::GetForwardVector()
{
	return this->vec_forward;
}

const DirectX::XMVECTOR& GameObject::GetRightVector()
{
	return this->vec_right;
}

const DirectX::XMVECTOR& GameObject::GetLeftVector()
{
	return this->vec_left;
}

const DirectX::XMVECTOR& GameObject::GetBackwardVector()
{
	return this->vec_backward;
}

const DirectX::XMVECTOR& GameObject::GetUpVector()
{
	return this->vec_up;
}

const DirectX::XMVECTOR& GameObject::GetDownVector()
{
	return this->vec_down;
}
