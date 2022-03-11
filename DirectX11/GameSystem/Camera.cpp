#include "Camera.h"

Camera::Camera()
{
	this->setTransform(std::make_shared<Transform>());
	this->mTransform->setPosition(0.0f, 0.0f, 0.0f);
	this->mTransform->setRotation(0.0f, 0.0f, 0.0f);

	this->updateMatrix();
}

void Camera::SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 360.0f) * DirectX::XM_2PI;
	this->projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);

	this->updateMatrix();
}

const DirectX::XMMATRIX& Camera::GetViewMatrix() const
{
	return this->viewMatrix;
}

const DirectX::XMMATRIX& Camera::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

void Camera::updateMatrix()
{
	DirectX::XMMATRIX camRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw
	(
		this->mTransform->getWorldRotation().x,
		this->mTransform->getWorldRotation().y,
		this->mTransform->getWorldRotation().z
	);
	
	DirectX::XMVECTOR camTarget = DirectX::XMVector3TransformCoord(this->mTransform->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	camTarget = DirectX::XMVectorAdd(camTarget, DirectX::XMLoadFloat3(&mTransform->getWorldPosition()));

	DirectX::XMVECTOR upDir = DirectX::XMVector3TransformCoord(this->mTransform->DEFAULT_UP_VECTOR, camRotationMatrix);
	this->viewMatrix = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&mTransform->getWorldPosition()), camTarget, upDir);

	DirectX::XMMATRIX vecRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(0.0f, this->mTransform->getWorldRotation().y, 0.0f);
	this->mTransform->setForwardVector(DirectX::XMVector3TransformCoord(this->mTransform->DEFAULT_FORWARD_VECTOR, vecRotationMatrix));
	this->mTransform->setRightVector(DirectX::XMVector3TransformCoord(this->mTransform->DEFAULT_RIGHT_VECTOR, vecRotationMatrix));
	this->mTransform->setLeftVector(DirectX::XMVector3TransformCoord(this->mTransform->DEFAULT_LEFT_VECTOR, vecRotationMatrix));
	this->mTransform->setBackwardVector(DirectX::XMVector3TransformCoord(this->mTransform->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix));
	this->mTransform->setUpVector(DirectX::XMVector3TransformCoord(this->mTransform->DEFAULT_UP_VECTOR, vecRotationMatrix));
	this->mTransform->setDownVector(DirectX::XMVector3TransformCoord(this->mTransform->DEFAULT_DOWN_VECTOR, vecRotationMatrix));
}
