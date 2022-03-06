#include "Transform.h"

void Transform::updateMatrix()
{
	this->mWorldMatrix = DirectX::XMMatrixMultiply
	(
		DirectX::XMMatrixRotationRollPitchYaw
		(
			this->mWorldRotation.x,
			this->mWorldRotation.y,
			this->mWorldRotation.z
		),
		DirectX::XMMatrixTranslation
		(
			this->mWorldPosition.x,
			this->mWorldPosition.y,
			this->mWorldPosition.z
		)
	);

	DirectX::XMMATRIX vecRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(0.0f, this->mWorldRotation.y, 0.0f);
	this->mForward = DirectX::XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	this->mRight = DirectX::XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
	this->mLeft = DirectX::XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	this->mBack = DirectX::XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	this->mUp = DirectX::XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, vecRotationMatrix);
	this->mDown = DirectX::XMVector3TransformCoord(this->DEFAULT_DOWN_VECTOR, vecRotationMatrix);
}

const DirectX::XMFLOAT3& Transform::getWorldPosition()
{
	return this->mWorldPosition;
}

const DirectX::XMFLOAT3& Transform::getWorldRotation()
{
	return this->mWorldRotation;
}

const DirectX::XMFLOAT3& Transform::getWorldScaleFactor()
{
	return this->mWorldScaleFactor;
}

void Transform::setPosition(const DirectX::XMVECTOR& position)
{
	DirectX::XMStoreFloat3(&this->mWorldPosition, position);
	this->updateMatrix();
}

void Transform::setPosition(const DirectX::XMFLOAT3& position)
{
	this->mWorldPosition = position;
	this->updateMatrix();
}

void Transform::setPosition(float x, float y, float z)
{
	this->mWorldPosition.x = x;
	this->mWorldPosition.y = y;
	this->mWorldPosition.z = z;
	this->updateMatrix();
}

void Transform::translate(const DirectX::XMVECTOR& pos)
{
	DirectX::XMStoreFloat3(&this->mWorldPosition, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&this->mWorldPosition), pos));
	this->updateMatrix();
}

void Transform::translate(const DirectX::XMFLOAT3& pos)
{
	this->mWorldPosition.x += pos.x;
	this->mWorldPosition.y += pos.y;
	this->mWorldPosition.z += pos.z;
	this->updateMatrix();
}

void Transform::translate(float x, float y, float z)
{
	this->mWorldPosition.x += x;
	this->mWorldPosition.y += y;
	this->mWorldPosition.z += z;
	this->updateMatrix();
}

void Transform::setRotation(const DirectX::XMVECTOR& rot)
{
	DirectX::XMStoreFloat3(&this->mWorldRotation, rot);
	this->updateMatrix();
}

void Transform::setRotation(const DirectX::XMFLOAT3& rot)
{
	this->mWorldRotation.x = rot.x;
	this->mWorldRotation.y = rot.y;
	this->mWorldRotation.z = rot.z;
	this->updateMatrix();
}

void Transform::setRotation(float x, float y, float z)
{
	this->mWorldRotation.x = x;
	this->mWorldRotation.y = y;
	this->mWorldRotation.z = z;
	this->updateMatrix();
}

void Transform::rotate(const DirectX::XMVECTOR& rot)
{
	DirectX::XMStoreFloat3(&this->mWorldRotation, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&this->mWorldRotation), rot));
	this->updateMatrix();
}

void Transform::rotate(const DirectX::XMFLOAT3& rot)
{
	this->mWorldRotation.x += rot.x;
	this->mWorldRotation.y += rot.y;
	this->mWorldRotation.z += rot.z;
	this->updateMatrix();
}

void Transform::rotate(float x, float y, float z)
{
	this->mWorldRotation.x += x;
	this->mWorldRotation.y += y;
	this->mWorldRotation.z += z;
	this->updateMatrix();
}

const DirectX::XMMATRIX& Transform::getWorldMatrix()
{
	return this->mWorldMatrix;
}

void Transform::setLookAtPos(DirectX::XMFLOAT3 lookAtPos)
{
	// lookat 위치와 카메라 위치 같음 체크
	if (lookAtPos.x == this->mWorldPosition.x && lookAtPos.y == this->mWorldPosition.y && lookAtPos.z == this->mWorldPosition.z)
	{
		return;
	}

	lookAtPos.x = this->mWorldPosition.x - lookAtPos.x;
	lookAtPos.y = this->mWorldPosition.y - lookAtPos.y;
	lookAtPos.z = this->mWorldPosition.z - lookAtPos.z;

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

	this->setRotation(pitch, yaw, 0.0f);
}

const DirectX::XMVECTOR& Transform::getForwardVector()
{
	this->mForward;
}

const DirectX::XMVECTOR& Transform::getRightVector()
{
	return mRight;
}

const DirectX::XMVECTOR& Transform::getLeftVector()
{
	return mLeft;
}

const DirectX::XMVECTOR& Transform::getBackwardVector()
{
	return mBack;
}

const DirectX::XMVECTOR& Transform::getUpVector()
{
	return mUp;
}

const DirectX::XMVECTOR& Transform::getDownVector()
{
	return mDown;
}

void Transform::setForwardVector(const DirectX::XMVECTOR& forward)
{
	this->mForward = forward;
}

void Transform::setRightVector(const DirectX::XMVECTOR& right)
{
	this->mForward = right;
}

void Transform::setLeftVector(const DirectX::XMVECTOR& left)
{
	this->mForward = left;
}

void Transform::setBackwardVector(const DirectX::XMVECTOR& backward)
{
	this->mForward = backward;
}

void Transform::setUpVector(const DirectX::XMVECTOR& up)
{
	this->mForward = up;
}

void Transform::setDownVector(const DirectX::XMVECTOR& down)
{
	this->mForward = down;
}
