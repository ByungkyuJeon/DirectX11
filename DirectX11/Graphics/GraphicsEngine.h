#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include "ConstantBufferTypes.h"
#include "..\\FrameTimer.h"
#include "Model.h"
#include "RenderableObjects.h"
#include "../GameSystem/Camera.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>
#include <vector>

class GraphicsEngine
{
public:
	bool Initialize(HWND hwnd, int width, int height);
	void RenderFrame();
	//RenderableObjects actor;

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();

	void registerRenderableObject(std::shared_ptr<Transform> transform, std::shared_ptr<Model> model);

	void setCamera(std::shared_ptr<Camera> camera);
	std::shared_ptr<Camera> getCamera();

private:
	bool InitializeDirectX(HWND hwnd);
	bool InitializeShaders();
	bool InitializeScene();

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	
	// 셰이더
	VertexShader vertexShader; 
	PixelShader pixelShader;
	ConstantBuffer<CB_VS_vertexshader> cb_vs_vertexShader;
	ConstantBuffer<CB_PS_pixelshader> cb_ps_pixelShader;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> raterizerState;
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendState;

	// 폰트
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	// 텍스처
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Texture_1;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Texture_2;

	int windowWidth = 0;
	int windowHeight = 0;

	FrameTimer frameTimer;

	// 렌더링이 필요한 오브젝트들
	std::vector<RenderableObjects> renderableObjects;

	// 카메라
	std::shared_ptr<Camera> mCamera;
};