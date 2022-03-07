#pragma once

#include "../CoordinateSystem/Transform.h"
#include "Model.h"
#include <string>
#include <memory>

class RenderableObjects
{
public:
	RenderableObjects() = delete;
	RenderableObjects(std::shared_ptr<Transform> transform, std::shared_ptr<Model> model);

	bool Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexShader);
	void Draw(ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexShader, const DirectX::XMMATRIX& viewProjectionMatrix);

	std::shared_ptr<Transform> getTransform();
	std::shared_ptr<Model> getModel();

	void setTransform(std::shared_ptr<Transform> transform);
	void setModel(std::shared_ptr<Model> model);

protected:


private:
	std::shared_ptr<Transform> mTransform;
	std::shared_ptr<Model> mModel;
	void UpdateMatrix();
};