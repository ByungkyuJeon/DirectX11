#include "RenderableObjects.h"

RenderableObjects::RenderableObjects(std::shared_ptr<Transform> transform, std::shared_ptr<Model> model) :
	mTransform{ transform }, mModel{ model }
{
}

bool RenderableObjects::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexShader)
{
	return true;
}

void RenderableObjects::Draw(ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexShader, const DirectX::XMMATRIX& viewProjectionMatrix)
{
	deviceContext->VSSetConstantBuffers(0, 1, cb_vs_vertexShader.GetAddressOf());
	for (int i = 0; i < this->mModel->getMeshes().size(); i++)
	{
		cb_vs_vertexShader.data.mat = DirectX::XMMatrixMultiply(this->mModel->getMeshAt(i).GetTransformMatrix(), DirectX::XMMatrixMultiply(this->mTransform->getWorldMatrix(), viewProjectionMatrix));
		cb_vs_vertexShader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexShader.data.mat);
		cb_vs_vertexShader.ApplyChanges();
		
		UINT offset = 0;

		for (int idx = 0; idx < this->mModel->getMeshAt(i).getTextures().size(); idx++)
		{
			if (this->mModel->getMeshAt(i).getTextureAt(idx).GetType() == aiTextureType::aiTextureType_DIFFUSE)
			{
				deviceContext->PSSetShaderResources(0, 1, this->mModel->getMeshAt(i).getTextureAt(idx).GetTextureResourceViewAddress());
				break;
			}
		}

		deviceContext->IASetVertexBuffers(0, 1, this->mModel->getMeshAt(i).getVertexBuffer().GetAddressOf(), this->mModel->getMeshAt(i).getVertexBuffer().StridePtr(), &offset);
		deviceContext->IASetIndexBuffer(this->mModel->getMeshAt(i).getIndxBuffer().Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
		deviceContext->DrawIndexed(this->mModel->getMeshAt(i).getIndxBuffer().IndexCount(), 0, 0);
	}
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