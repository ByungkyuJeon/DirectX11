#include "Camera.h"

Camera::Camera()
{
	this->pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->posVector = XMLoadFloat3(&this->pos);
	this->rot = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotVector = XMLoadFloat3(&this->rot);

	this->UpdateMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * DirectX::XM_2PI;
	this->projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
}

const DirectX::XMMATRIX& Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const DirectX::XMMATRIX& Camera::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

void Camera::UpdateMatrix()
{
	DirectX::XMMATRIX camRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z);
	
	DirectX::XMVECTOR camTarget = DirectX::XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	camTarget = DirectX::XMVectorAdd(camTarget, this->posVector);

	DirectX::XMVECTOR upDir = DirectX::XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
	this->viewMatrix = DirectX::XMMatrixLookAtLH(this->posVector, camTarget, upDir);

	DirectX::XMMATRIX vecRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
	this->vec_forward = DirectX::XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	this->vec_right = DirectX::XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
	this->vec_left = DirectX::XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	this->vec_backward = DirectX::XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	this->vec_up = DirectX::XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, vecRotationMatrix);
	this->vec_down = DirectX::XMVector3TransformCoord(this->DEFAULT_DOWN_VECTOR, vecRotationMatrix);
}
