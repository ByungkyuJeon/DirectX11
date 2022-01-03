#include "RenderableGameObject.h"

bool RenderableGameObject::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexShader)
{
	if (!this->model.Initialize(filePath, device, deviceContext, cb_vs_vertexShader))
	{
		return false;
	}

	this->pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->posVector = XMLoadFloat3(&this->pos);
	this->rot = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotVector = XMLoadFloat3(&this->rot);

	this->UpdateMatrix();

	return true;
}

void RenderableGameObject::Draw(const DirectX::XMMATRIX& viewProjectionMatrix)
{
	this->model.Draw(this->worldMatrix, viewProjectionMatrix);
}

void RenderableGameObject::UpdateMatrix()
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