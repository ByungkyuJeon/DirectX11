#include "RenderableObjects.h"

RenderableObjects::RenderableObjects(std::shared_ptr<Transform> transform, std::shared_ptr<Model> model) :
	mTransform{ transform }, mModel{ model }
{
}

bool RenderableObjects::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexShader)
{
	if (!this->mModel->Initialize(filePath, device, deviceContext, cb_vs_vertexShader))
	{
		return false;
	}

	this->UpdateMatrix();

	return true;
}

void RenderableObjects::Draw(const DirectX::XMMATRIX& viewProjectionMatrix)
{
	this->mModel->Draw(this->mTransform->getWorldMatrix(), viewProjectionMatrix);
}

std::shared_ptr<Transform> RenderableObjects::getTransform()
{
	return this->mTransform;
}

std::shared_ptr<Model> RenderableObjects::getModel()
{
	return this->mModel;
}

void RenderableObjects::setTransform(std::shared_ptr<Transform> transform)
{
	this->mTransform = transform;
}

void RenderableObjects::setModel(std::shared_ptr<Model> model)
{
	this->mModel = model;
}

void RenderableObjects::UpdateMatrix()
{
	this->mTransform->updateMatrix();
}