#include "RenderableObjects.h"

RenderableObjects::RenderableObjects(std::shared_ptr<GameObject> gameObject) :
	mGameObject{ gameObject }
{
}

bool RenderableObjects::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexShader)
{
	return true;
}

void RenderableObjects::Draw(ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexShader, const DirectX::XMMATRIX& viewProjectionMatrix)
{
	deviceContext->VSSetConstantBuffers(0, 1, cb_vs_vertexShader.GetAddressOf());
	for (int i = 0; i < this->mGameObject->getModel()->getMeshes().size(); i++)
	{
		cb_vs_vertexShader.data.mat = DirectX::XMMatrixMultiply(this->mGameObject->getModel()->getMeshAt(i).GetTransformMatrix(), DirectX::XMMatrixMultiply(this->mGameObject->getTransform()->getWorldMatrix(), viewProjectionMatrix));
		cb_vs_vertexShader.data.mat = DirectX::XMMatrixTranspose(cb_vs_vertexShader.data.mat);
		cb_vs_vertexShader.ApplyChanges();
		
		UINT offset = 0;

		for (int idx = 0; idx < this->mGameObject->getModel()->getMeshAt(i).getTextures().size(); idx++)
		{
			if (this->mGameObject->getModel()->getMeshAt(i).getTextureAt(idx).GetType() == aiTextureType::aiTextureType_DIFFUSE)
			{
				deviceContext->PSSetShaderResources(0, 1, this->mGameObject->getModel()->getMeshAt(i).getTextureAt(idx).GetTextureResourceViewAddress());
				break;
			}
		}

		deviceContext->IASetVertexBuffers(0, 1, this->mGameObject->getModel()->getMeshAt(i).getVertexBuffer().GetAddressOf(), this->mGameObject->getModel()->getMeshAt(i).getVertexBuffer().StridePtr(), &offset);
		deviceContext->IASetIndexBuffer(this->mGameObject->getModel()->getMeshAt(i).getIndxBuffer().Get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
		deviceContext->DrawIndexed(this->mGameObject->getModel()->getMeshAt(i).getIndxBuffer().IndexCount(), 0, 0);
	}
}

std::shared_ptr<GameObject> RenderableObjects::getGameObject()
{
	return std::shared_ptr<GameObject>();
}

std::shared_ptr<Transform> RenderableObjects::getTransform()
{
	return this->mGameObject->getTransform();
}

std::shared_ptr<Model> RenderableObjects::getModel()
{
	return this->mGameObject->getModel();
}