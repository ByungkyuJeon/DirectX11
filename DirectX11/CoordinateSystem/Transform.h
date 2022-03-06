#pragma once

#include <DirectXMath.h>

class Transform
{
public:
	Transform() = default;

	void updateMatrix();

	const DirectX::XMFLOAT3& getWorldPosition();
	const DirectX::XMFLOAT3& getWorldRotation();
	const DirectX::XMFLOAT3& getWorldScaleFactor();

	//const DirectX::XMFLOAT3& getLocalPosition();
	//const DirectX::XMFLOAT3& getLocalRotation();
	//const DirectX::XMFLOAT3& getLocalScaleFactor();

	void setPosition(const DirectX::XMVECTOR& position);
	void setPosition(const DirectX::XMFLOAT3& position);
	void setPosition(float x, float y, float z);
	void translate(const DirectX::XMVECTOR& pos);
	void translate(const DirectX::XMFLOAT3& pos);
	void translate(float x, float y, float z);

	void setRotation(const DirectX::XMVECTOR& rot);
	void setRotation(const DirectX::XMFLOAT3& rot);
	void setRotation(float x, float y, float z);
	void rotate(const DirectX::XMVECTOR& rot);
	void rotate(const DirectX::XMFLOAT3& rot);
	void rotate(float x, float y, float z);

	void setLookAtPos(DirectX::XMFLOAT3 lookAtPos);

	const DirectX::XMVECTOR& getForwardVector();
	const DirectX::XMVECTOR& getRightVector();
	const DirectX::XMVECTOR& getLeftVector();
	const DirectX::XMVECTOR& getBackwardVector();
	const DirectX::XMVECTOR& getUpVector();
	const DirectX::XMVECTOR& getDownVector();
	void setForwardVector(const DirectX::XMVECTOR& forward);
	void setRightVector(const DirectX::XMVECTOR& right);
	void setLeftVector(const DirectX::XMVECTOR& left);
	void setBackwardVector(const DirectX::XMVECTOR& backward);
	void setUpVector(const DirectX::XMVECTOR& up);
	void setDownVector(const DirectX::XMVECTOR& down);

	const DirectX::XMMATRIX& getWorldMatrix();

	const DirectX::XMVECTOR DEFAULT_FORWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_UP_VECTOR = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_DOWN_VECTOR = DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_BACKWARD_VECTOR = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_LEFT_VECTOR = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_RIGHT_VECTOR = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

private:

	DirectX::XMMATRIX mWorldMatrix = DirectX::XMMatrixIdentity();

	DirectX::XMFLOAT3 mWorldPosition;
	DirectX::XMFLOAT3 mWorldRotation;
	DirectX::XMFLOAT3 mWorldScaleFactor;

	//DirectX::XMFLOAT3 mLocalPosition;
	//DirectX::XMFLOAT3 mLocalRotation;
	//DirectX::XMFLOAT3 mLocalScaleFactor;

	DirectX::XMVECTOR mForward;
	DirectX::XMVECTOR mLeft;
	DirectX::XMVECTOR mRight;
	DirectX::XMVECTOR mBack;
	DirectX::XMVECTOR mUp;
	DirectX::XMVECTOR mDown;
};