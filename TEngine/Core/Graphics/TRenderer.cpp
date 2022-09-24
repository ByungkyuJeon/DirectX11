#include "TRenderer.h"
#include "../TEngine.h"
#include "../Helper/StringHelper.h"
#include <memory>

bool TRenderer::InitializeDirectX()
{
	if(false == m_AdapterManager.Initialize())
	{
		PRINT_ERROR("adapter initialization failed.");
		return false;
	}

	if (false == InitializeSwapChain())
	{
		PRINT_ERROR("swap chain initialization failed.");
		return false;
	}

	if (false == InitializeRenderTargetView())
	{
		PRINT_ERROR("render target view initialization failed.");
		return false;
	}

	if (false == InitializeDepthStencil())
	{
		PRINT_ERROR("depth stencil initialization failed.");
		return false;
	}

	if (false == InitializeOutputMerger())
	{
		PRINT_ERROR("output merger initialization failed.");
		return false;
	}

	if (false == InitializeViewPort())
	{
		PRINT_ERROR("viewport initialization failed.");
		return false;
	}

	if (false == InitializeRasterizer())
	{
		PRINT_ERROR("rasterizer initialization failed.");
		return false;
	}

	if (false == InitializeBlendState())
	{
		PRINT_ERROR("blend state initialization failed.");
		return false;
	}

	if (false == InitializeSamplerState())
	{
		PRINT_ERROR("sampler state initialization failed.");
		return false;
	}

	// 정리 덜 된 초기화 코드

	// 스프라이트 폰트 설정
	m_SpriteBatch = std::make_unique<DirectX::SpriteBatch>(GDeviceContext.Get());
	m_SpriteFont = std::make_unique<DirectX::SpriteFont>(GDevice.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");

    return true;
}

bool TRenderer::InitializeSwapChain()
{
	HRESULT hResult;

	// 스왑 체인 생성
	hResult = D3D11CreateDeviceAndSwapChain(
		m_AdapterManager.GetSelectedAdpaterData()->GetIDXGIAdapter(),
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		NULL,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&m_SwapChain.GetSwapChainDesc(m_HWND, ExEngine->GetWindowWidth(), ExEngine->GetWindowHeight()),
		m_SwapChain.GetSwapChain().GetAddressOf(),
		GDevice.GetAddressOf(),
		NULL,
		GDeviceContext.GetAddressOf()
	);

	if (FAILED(hResult))
	{
		PRINT_ERROR("swap chain creation failed.")
		return false;
	}

	// 백버퍼 세팅
	hResult = m_SwapChain.GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(m_SwapChain.GetBackBuffer().GetAddressOf()));
	if (FAILED(hResult))
	{
		PRINT_ERROR("swap chain back buffer obtain failed.")
		return false;
	}

	return true;
}

bool TRenderer::InitializeRenderTargetView()
{
	HRESULT hResult;

	// 렌더 타겟 뷰 생성
	hResult = GDevice->CreateRenderTargetView(m_SwapChain.GetBackBuffer().Get(), NULL, m_RenderTargetView.GetAddressOf());
	if (FAILED(hResult))
	{
		PRINT_ERROR("render target view creation failed")
		return false;
	}

	return true;
}

bool TRenderer::InitializeDepthStencil()
{
	HRESULT hResult;

	// 뎁스 스탠실 버퍼 생성
	hResult = GDevice->CreateTexture2D(&m_DepthStencil.GetDepthStencilDesc(ExEngine->GetWindowWidth(), ExEngine->GetWindowHeight()), NULL, m_DepthStencil.GetDepthStencilBuffer().GetAddressOf());
	if (FAILED(hResult))
	{
		PRINT_ERROR("depth stencil buffer creation failed.")
		return false;
	}

	// 뎁스 스탠실 뷰 생성
	hResult = GDevice->CreateDepthStencilView(m_DepthStencil.GetDepthStencilBuffer().Get(), NULL, m_DepthStencil.GetDepthStencilView().GetAddressOf());
	if (FAILED(hResult))
	{
		PRINT_ERROR("depth stencil view creation failed.")
		return false;
	}

	hResult = GDevice->CreateDepthStencilState(&m_DepthStencil.GetDepthStencilStateDesc(), m_DepthStencil.GetDepthStencilState().GetAddressOf());
	if (FAILED(hResult))
	{
		PRINT_ERROR("depth stencil state creation failed.")
		return false;
	}

	return true;
}

bool TRenderer::InitializeOutputMerger()
{
	// output merger 의 렌더 타겟 설정
	GDeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), m_DepthStencil.GetDepthStencilView().Get());

	return true;
}

bool TRenderer::InitializeViewPort()
{
	GDeviceContext->RSSetViewports(1, &m_ViewPort.GetViewPort(ExEngine->GetWindowWidth(), ExEngine->GetWindowHeight()));

	return true;
}

bool TRenderer::InitializeRasterizer()
{
	HRESULT hResult;

	hResult = GDevice->CreateRasterizerState(&m_Rasterizer.GetRasterizerDesc(), m_Rasterizer.GetRasterizerState().GetAddressOf());
	if (FAILED(hResult))
	{
		PRINT_ERROR("raterizer state creation failed.")
		return false;
	}

	return true;
}

bool TRenderer::InitializeBlendState()
{
	HRESULT hResult;

	hResult = GDevice->CreateBlendState(&m_BlendState.GetBlendDesc(), m_BlendState.GetBlendState().GetAddressOf());
	if (FAILED(hResult))
	{
		PRINT_ERROR("blend state creation failed.")
		return false;
	}

	return true;
}

bool TRenderer::InitializeSamplerState()
{
	HRESULT hResult;

	hResult = GDevice->CreateSamplerState(&m_SamplerState.GetSamplerDesc(), m_SamplerState.GetSamplerState().GetAddressOf());
	if (FAILED(hResult))
	{
		PRINT_ERROR("sampler state creation failed.")
		return false;
	}

	return true;
}

bool TRenderer::InitializeShaders()
{
	TWString shaderfolder;
#pragma region DetermineShaderPath
	if (IsDebuggerPresent() == TRUE)
	{
#ifdef _DEBUG 
	#ifdef _WIN64
			shaderfolder = L"..\\x64\\Debug\\";
	#else
			shaderfolder = L"..\\Debug\\";
	#endif
#else
	#ifdef _WIN64
			shaderfolder = L"..\\x64\\Release\\";
	#else
			shaderfolder = L"..\\Release\\";
	#endif
#endif
	}

	D3D11_INPUT_ELEMENT_DESC layout[]
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Offset macro : D3D11_APPEND_ALIGNED_ELEMENT
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT numElements = ARRAYSIZE(layout);

	if (!m_VertexShader.Initialize(shaderfolder + L"vertexShader.cso", layout, numElements))
	{
		return false;
	}

	if (!m_PixelShader.Initialize(shaderfolder + L"pixelShader.cso"))
	{
		return false;
	}

	return true;
}

bool TRenderer::InitializeScene()
{
	HRESULT hResult;

	hResult = DirectX::CreateWICTextureFromFile(GDevice.Get(), L"Data\\Textures\\Ivan_1304_DIFF.jpg", nullptr, Texture_1.GetAddressOf());
	if (FAILED(hResult))
	{
		PRINT_ERROR("CreateWICTextureFromFIle failed.");
		return false;
	}

	hResult = DirectX::CreateWICTextureFromFile(GDevice.Get(), L"Data\\Textures\\testTexture2.png", nullptr, Texture_2.GetAddressOf());
	if (FAILED(hResult))
	{
		PRINT_ERROR("CreateWICTextureFromFIle failed.");
		return false;
	}

	hResult = cb_vs_vertexShader.Initialize(GDevice.Get(), GDeviceContext.Get());
	if (FAILED(hResult))
	{
		PRINT_ERROR("vertex constant buffer initialization failed.");
		return false;
	}

	hResult = cb_ps_light.Initialize(GDevice.Get(), GDeviceContext.Get());
	if (FAILED(hResult))
	{
		PRINT_ERROR("pixel constant buffer initialization failed.");
		return false;
	}

	this->cb_ps_light.data.ambientLightColor = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	this->cb_ps_light.data.ambientLightStrength = 1.0f;

	// 모델 초기화
	/*if (!actor.Initialize("Data\\Objects\\car.fbx", GDevice.Get(), GDeviceContext.Get(), this->cb_vs_vertexShader))
	{
		return false;
	}*/

	if (mCamera == nullptr)
	{
		mCamera = std::make_shared<Camera>();
		mCamera->getTransform()->setPosition(0.0f, 0.0f, -2.0f);
		mCamera->SetProjectionValues(90.0f, static_cast<float>(ExEngine->GetWindowWidth()) / static_cast<float>(ExEngine->GetWindowHeight()), 0.1f, 1000.0f);
	}

	return true;
}

bool TRenderer::Initialize()
{
    if (!InitializeDirectX())
    {
        PRINT_ERROR("DirectX initialization failed.")
        return false;
    }

	if (false == InitializeShaders())
	{
		PRINT_ERROR("sampler state initialization failed.");
		return false;
	}

	if (false == InitializeScene())
	{
		PRINT_ERROR("sampler state initialization failed.");
		return false;
	}

    return true;
}

void TRenderer::RegisterRenderableObject(std::shared_ptr<GameObject> InGameObject)
{
	RenderableObjects renderableObject = RenderableObjects(InGameObject);

	// 환경 물리 연산 대상에 추가
	this->renderableObjects.emplace_back(renderableObject);
}

void TRenderer::RenderTick()
{
	this->cb_ps_light.ApplyChanges();
	GDeviceContext->PSSetConstantBuffers(0, 1, this->cb_ps_light.GetAddressOf());

	float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GDeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), bgcolor);
	GDeviceContext->ClearDepthStencilView(m_DepthStencil.GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH || D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Input Assembler Stage
	GDeviceContext->IASetInputLayout(m_VertexShader.GetInputLayout().Get());
	GDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//D3D10_PRIMITIVE_TOPOLOGY_POINTLIST
	//D3D10_PRIMITIVE_TOPOLOGY_LINELIST
	//D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST

	// Vertex Shader Stage
	GDeviceContext->VSSetShader(m_VertexShader.GetShader().Get(), NULL, 0);

	// Hull Shader
	// Tesselator
	// Domain Shader
	// Geometry Shader
	// Rasterizer
	GDeviceContext->RSSetState(m_Rasterizer.GetRasterizerState().Get());
	// Pixel Shader
	GDeviceContext->PSSetSamplers(0, 1, m_SamplerState.GetSamplerState().GetAddressOf());
	GDeviceContext->PSSetShader(m_PixelShader.GetShader().Get(), NULL, 0);
	// Output Merger
	GDeviceContext->OMSetDepthStencilState(m_DepthStencil.GetDepthStencilState().Get(), 0);
	GDeviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF); // this->blendState.Get()



	UINT offset = 0;

	// Draw Renderable Objects
	for (auto& renderable : this->renderableObjects)
	{
		renderable.Draw(GDeviceContext.Get(), cb_vs_vertexShader, DirectX::XMMatrixMultiply(this->mCamera->GetViewMatrix(), this->mCamera->GetProjectionMatrix()));
	}
	//this->actor.Draw(DirectX::XMMatrixMultiply(this->camera.GetViewMatrix(), this->camera.GetProjectionMatrix()));

	// Pixel shader constant buffer
	/*static float alpha = 0.5f;
	this->cb_ps_pixelShader.data.alpha = alpha;
	if (!cb_ps_pixelShader.ApplyChanges())
	{
		return;
	}
	GDeviceContext->PSSetConstantBuffers(0, 1, cb_ps_pixelShader.GetAddressOf());*/

	static int frameTime = 0;
	static std::string frameTimeString = "FPS : 0";
	frameTime += 1;
	if (m_FrameTimer.GetMilisecondsElapsed() > 500.0)
	{
		frameTimeString = "FPS : " + std::to_string(frameTime * 2);
		frameTime = 0;
		m_FrameTimer.ReStart();
	}

	m_SpriteBatch->Begin();
	m_SpriteFont->DrawString(m_SpriteBatch.get(), StringHelper::StringToWide(frameTimeString).c_str(), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
	m_SpriteBatch->End();

	// vsync on : 1
	// vsync off : 0
	m_SwapChain.GetSwapChain()->Present(1, NULL);
}

TSwapChain::TSwapChain()
{
	ZeroMemory(&m_SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	m_SwapChainDesc.BufferDesc.Width = DEFAULT_WINDOW_WIDTH;
	m_SwapChainDesc.BufferDesc.Height = DEFAULT_WINDOW_HEIGHT;
	m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	m_SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	m_SwapChainDesc.SampleDesc.Count = 1;
	m_SwapChainDesc.SampleDesc.Quality = 0;

	m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// 1 - 더블 버퍼링
	// 2 - 트리플 버퍼링
	m_SwapChainDesc.BufferCount = 1;
	m_SwapChainDesc.OutputWindow = INVALID_HANDLE;
	m_SwapChainDesc.Windowed = true;
	m_SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
}

DXGI_SWAP_CHAIN_DESC& TSwapChain::GetSwapChainDesc(HWND InHwnd /*= INVALID_HANDLE*/, Length InWindowWidth /*= INVALID_LENGTH*/, Length InWindowHeight /*= INVALID_LENGTH*/)
{
	if (InHwnd != INVALID_HANDLE)
		m_SwapChainDesc.OutputWindow = InHwnd;

	if (InHwnd == INVALID_HANDLE)
		PRINT_ERROR("핸들이 세팅되지 않았음!!")

	if(InWindowWidth != INVALID_LENGTH)
		m_SwapChainDesc.BufferDesc.Width = InWindowWidth;
	
	if (InWindowHeight != INVALID_LENGTH)
		m_SwapChainDesc.BufferDesc.Height = InWindowHeight;

	return m_SwapChainDesc;
}

TDepthStencil::TDepthStencil() :
	m_DepthStencilStateDesc( D3D11_DEFAULT )
{
	m_DepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	m_DepthStencilDesc.Width = DEFAULT_WINDOW_WIDTH;
	m_DepthStencilDesc.Height = DEFAULT_WINDOW_HEIGHT;
	m_DepthStencilDesc.MipLevels = 1;
	m_DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	m_DepthStencilStateDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
}

CD3D11_TEXTURE2D_DESC& TDepthStencil::GetDepthStencilDesc(Length InWindowWidth /*= INVALID_LENGTH*/, Length InWindowHeight /*= INVALID_LENGTH*/)
{
	if (InWindowWidth != INVALID_LENGTH)
		m_DepthStencilDesc.Width = InWindowWidth;

	if (InWindowHeight != INVALID_LENGTH)
		m_DepthStencilDesc.Height = InWindowHeight;

	return m_DepthStencilDesc;
}

TViewPort::TViewPort()
{
	m_ViewPort.TopLeftX = 0.0f;
	m_ViewPort.TopLeftY = 0.0f;
	m_ViewPort.MinDepth = D3D11_MIN_DEPTH;
	m_ViewPort.MaxDepth = D3D11_MAX_DEPTH;
}

CD3D11_VIEWPORT& TViewPort::GetViewPort(Length InWindowWidth /*= INVALID_LENGTH*/, Length InWindowHeight /*= INVALID_LENGTH*/)
{
	if (InWindowWidth != INVALID_LENGTH)
		m_ViewPort.Width = InWindowWidth;

	if (InWindowHeight != INVALID_LENGTH)
		m_ViewPort.Height = InWindowHeight;

	return m_ViewPort;
}

TRasterizer::TRasterizer()
{
	ZeroMemory(&m_RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	m_RasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	m_RasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
}

TBlendState::TBlendState()
{
	ZeroMemory(&m_RenderTargetBlendDesc, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));

	m_RenderTargetBlendDesc.BlendEnable = true;
	m_RenderTargetBlendDesc.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	m_RenderTargetBlendDesc.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	m_RenderTargetBlendDesc.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	m_RenderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	m_RenderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	m_RenderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	m_RenderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	ZeroMemory(&m_BlendDesc, sizeof(D3D11_BLEND_DESC));

	m_BlendDesc.RenderTarget[0] = m_RenderTargetBlendDesc;
}

TSamplerState::TSamplerState()
{
	ZeroMemory(&m_SamplerDesc, sizeof(D3D11_SAMPLER_DESC));

	m_SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	m_SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	m_SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	m_SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	m_SamplerDesc.MinLOD = 0;
	m_SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
}
