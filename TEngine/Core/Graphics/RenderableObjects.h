#pragma once

#include "../Framework/Game/GameObject.h"
#include <string>
#include <memory>

class RenderableObjects
{
public:
	RenderableObjects() = delete;
	RenderableObjects(std::shared_ptr<GameObject> gameObject);

	bool Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexShader);
	void Draw(ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexShader, const DirectX::XMMATRIX& viewProjectionMatrix);

	std::shared_ptr<GameObject> getGameObject();
	std::shared_ptr<Transform> getTransform();
	std::shared_ptr<Model> getModel();

protected:


private:
	std::shared_ptr<GameObject> mGameObject;
};