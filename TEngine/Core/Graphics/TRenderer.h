#pragma once

#include "../Core.h"
#include "TAdapterManager.h"
#include "Shaders/Shaders.h"
#include "Shaders/ConstantBuffer.h"
#include "../Framework/Game/Camera.h"
#include "RenderableObjects.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>

class TSwapChain
{
private:
	DXGI_SWAP_CHAIN_DESC m_SwapChainDesc;

	TComPtr<IDXGISwapChain> m_SwapChain;
	TComPtr<ID3D11Texture2D> m_BackBuffer;

public:
	TSwapChain();
	TSwapChain(TSwapChain& InOther) = delete;
	TSwapChain(TSwapChain&& InOther) = delete;
	TSwapChain& operator=(TSwapChain& InOther) = delete;
	TSwapChain&& operator=(TSwapChain&& InOhter) = delete;

	DXGI_SWAP_CHAIN_DESC& GetSwapChainDesc(HWND InHwnd = INVALID_HANDLE, Length InWindowWidth = INVALID_LENGTH, Length InWindowHeight = INVALID_LENGTH);

	TComPtr<IDXGISwapChain> GetSwapChain() { return m_SwapChain; }
	TComPtr<ID3D11Texture2D> GetBackBuffer() { return m_BackBuffer; }
};

class TDepthStencil
{
private:
	CD3D11_TEXTURE2D_DESC m_DepthStencilDesc;
	CD3D11_DEPTH_STENCIL_DESC m_DepthStencilStateDesc;

	TComPtr<ID3D11DepthStencilView> m_DepthStencilView;
	TComPtr<ID3D11Texture2D> m_DepthStencilBuffer;
	TComPtr<ID3D11DepthStencilState> m_DepthStencilState;

public:
	TDepthStencil();
	TDepthStencil(TDepthStencil& InOther) = delete;
	TDepthStencil(TDepthStencil&& InOther) = delete;
	TDepthStencil& operator=(TDepthStencil& InOther) = delete;
	TDepthStencil& operator=(TDepthStencil&& InOther) = delete;

	CD3D11_TEXTURE2D_DESC& GetDepthStencilDesc(Length InWindowWidth = INVALID_LENGTH, Length InWindowHeight = INVALID_LENGTH);
	CD3D11_DEPTH_STENCIL_DESC& GetDepthStencilStateDesc() { return m_DepthStencilStateDesc; }

	TComPtr<ID3D11DepthStencilView> GetDepthStencilView() { return m_DepthStencilView; }
	TComPtr<ID3D11Texture2D> GetDepthStencilBuffer() { return m_DepthStencilBuffer; }
	TComPtr<ID3D11DepthStencilState> GetDepthStencilState() { return m_DepthStencilState; }
};

class TViewPort
{
private:
	CD3D11_VIEWPORT m_ViewPort;

public:
	TViewPort();
	TViewPort(TViewPort& InOther) = delete;
	TViewPort(TViewPort&& InOther) = delete;
	TViewPort& operator=(TViewPort& InOther) = delete;
	TViewPort& operator=(TViewPort&& InOther) = delete;

	CD3D11_VIEWPORT& GetViewPort(Length InWindowWidth = INVALID_LENGTH, Length InWindowHeight = INVALID_LENGTH);
};

class TRasterizer
{
private:
	D3D11_RASTERIZER_DESC m_RasterizerDesc;
	TComPtr<ID3D11RasterizerState> m_RaterizerState;

public:
	TRasterizer();
	TRasterizer(TRasterizer& InOther) = delete;
	TRasterizer(TRasterizer&& InOther) = delete;
	TRasterizer& operator=(TRasterizer& InOther) = delete;
	TRasterizer& operator=(TRasterizer&& InOther) = delete;

	D3D11_RASTERIZER_DESC& GetRasterizerDesc() { return m_RasterizerDesc; };
	TComPtr<ID3D11RasterizerState> GetRasterizerState() { return m_RaterizerState; };
};

class TBlendState
{
private:
	D3D11_RENDER_TARGET_BLEND_DESC m_RenderTargetBlendDesc;
	D3D11_BLEND_DESC m_BlendDesc;

	TComPtr<ID3D11BlendState> m_BlendState;

public:
	TBlendState();
	TBlendState(TBlendState& InOther) = delete;
	TBlendState(TBlendState&& InOther) = delete;
	TBlendState& operator=(TBlendState& InOther) = delete;
	TBlendState& operator=(TBlendState&& InOther) = delete;

	D3D11_RENDER_TARGET_BLEND_DESC& GetRenderTargetBlendDesc() { return m_RenderTargetBlendDesc; }
	D3D11_BLEND_DESC& GetBlendDesc() { return m_BlendDesc; }
	TComPtr<ID3D11BlendState> GetBlendState() { return m_BlendState; }
};

class TSamplerState
{
private:
	D3D11_SAMPLER_DESC m_SamplerDesc;

	TComPtr<ID3D11SamplerState> m_SamplerState;

public:
	TSamplerState();
	TSamplerState(TSamplerState& InOther) = delete;
	TSamplerState(TSamplerState&& InOther) = delete;
	TSamplerState& operator=(TSamplerState& InOther) = delete;
	TSamplerState& operator=(TSamplerState&& InOther) = delete;

	D3D11_SAMPLER_DESC& GetSamplerDesc() { return m_SamplerDesc; }
	TComPtr<ID3D11SamplerState> GetSamplerState() { return m_SamplerState; }
};

class TRenderer
{
private:
	HWND m_HWND;

	TAdapterManager m_AdapterManager;
	TSwapChain m_SwapChain;
	TDepthStencil m_DepthStencil;
	TViewPort m_ViewPort;
	TRasterizer m_Rasterizer;
	TBlendState m_BlendState;
	TSamplerState m_SamplerState;

	TComPtr<ID3D11RenderTargetView> m_RenderTargetView;

	int m_SeclectedAdapterDataIndex = INVALID_INDEX;

	// 임시
	// 폰트
	std::unique_ptr<DirectX::SpriteBatch> m_SpriteBatch;
	std::unique_ptr<DirectX::SpriteFont> m_SpriteFont;

	// 셰이더
	TVertexShader m_VertexShader;
	TPixelShader m_PixelShader;
	ConstantBuffer<CB_VS_vertexshader> cb_vs_vertexShader;
	ConstantBuffer<CB_PS_light> cb_ps_light;

	// 텍스처
	TComPtr<ID3D11ShaderResourceView> Texture_1;
	TComPtr<ID3D11ShaderResourceView> Texture_2;

	TFrameTimer m_FrameTimer;

	// 렌더링이 필요한 오브젝트들
	std::vector<RenderableObjects> renderableObjects;

	// 카메라
	std::shared_ptr<Camera> mCamera;

	// 임시

	bool InitializeDirectX();

	bool InitializeSwapChain();
	bool InitializeRenderTargetView();
	bool InitializeDepthStencil();
	bool InitializeOutputMerger();
	bool InitializeViewPort();
	bool InitializeRasterizer();
	bool InitializeBlendState();
	bool InitializeSamplerState();
	
	bool InitializeShaders();
	bool InitializeScene();

public:
	bool Initialize();

	void RegisterRenderableObject(std::shared_ptr<GameObject> InGameObject);

	void RenderTick();

	std::shared_ptr<Camera> GetCamera() { return mCamera; };
};