#include "Graphics.h"

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
	frameTimer.Start();

	windowWidth = width;
	windowHeight = height;

	if (!InitializeDirectX(hwnd))
	{
		return false;
	}

	if (!InitializeShaders())
	{
		return false;
	}

	if (!InitializeScene())
	{
		return false;
	}

	return true;
}

void Graphics::RenderFrame()
{
	float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH || D3D11_CLEAR_STENCIL, 1.0f, 0);

	this->deviceContext->IASetInputLayout(this->vertexShader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//D3D10_PRIMITIVE_TOPOLOGY_POINTLIST
	//D3D10_PRIMITIVE_TOPOLOGY_LINELIST
	//D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	this->deviceContext->RSSetState(this->raterizerState.Get());
	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	this->deviceContext->OMSetBlendState(NULL, NULL, 0xFFFFFFFF); // this->blendState.Get()
	this->deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());

	this->deviceContext->VSSetShader(this->vertexShader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(this->pixelShader.GetShader(), NULL, 0);

	UINT offset = 0;

	// MODELS
	this->gameObject.Draw(DirectX::XMMatrixMultiply(this->camera.GetViewMatrix(), this->camera.GetProjectionMatrix()));

	// Pixel shader constant buffer
	/*static float alpha = 0.5f;
	this->cb_ps_pixelShader.data.alpha = alpha;
	if (!cb_ps_pixelShader.ApplyChanges())
	{
		return;
	}
	this->deviceContext->PSSetConstantBuffers(0, 1, cb_ps_pixelShader.GetAddressOf());*/

	static int frameTime = 0;
	static std::string frameTimeString = "FPS : 0";
	frameTime += 1;
	if (frameTimer.GetMilisecondsElapsed() > 500.0)
	{
		frameTimeString = "FPS : " + std::to_string(frameTime * 2);
		frameTime = 0;
		frameTimer.ReStart();
	}

	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch.get(), StringHelper::StringToWide(frameTimeString).c_str(), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
	spriteBatch->End();

	// vsync on : 1
	// vsync off : 0
	this->swapChain->Present(1, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd)
{
	try
	{
		std::vector<AdapterData> adapters = AdapterReader::GetAdapters();
		int selectedAdapterIdx = 0;

		// 인식된 어뎁터 체크
		if (adapters.size() < 1)
		{
			ErrorLogger::Log("No IDXGI Adapaters found.");
			return false;
		}
		else
		{
			// 비디오 메모리가 가장 큰 어뎁터 선택
			for (int adapterIdx = 0; adapterIdx < adapters.size(); adapterIdx++)
			{
				if (adapters[adapterIdx].description.DedicatedVideoMemory > adapters[selectedAdapterIdx].description.DedicatedVideoMemory)
				{
					selectedAdapterIdx = adapterIdx;
				}
			}
		}

		// 스왑 체인 구조체 설정
		DXGI_SWAP_CHAIN_DESC scd;
		ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

		scd.BufferDesc.Width = windowWidth;
		scd.BufferDesc.Height = windowHeight;
		scd.BufferDesc.RefreshRate.Numerator = 60;
		scd.BufferDesc.RefreshRate.Denominator = 1;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;

		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		// 1 - 더블 버퍼링
		// 2 - 트리플 버퍼링
		scd.BufferCount = 1;
		scd.OutputWindow = hwnd;
		scd.Windowed = true;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// 스왑 체인 오브젝트 생성
		HRESULT hr;
		hr = D3D11CreateDeviceAndSwapChain(
			adapters[selectedAdapterIdx].pAdapter,
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL,
			NULL,
			NULL,
			0,
			D3D11_SDK_VERSION,
			&scd,
			this->swapChain.GetAddressOf(),
			this->device.GetAddressOf(),
			NULL,
			this->deviceContext.GetAddressOf()
		);

		COM_ERROR_IF_FAILED(hr, "Failed to create device and swapchain.");

		// 스왑 체인 백버퍼 주소 읽어오기
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		hr = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		COM_ERROR_IF_FAILED(hr, "Failed GetBuffer.");

		// 렌더 타겟 뷰 생성
		hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "CreateRenderTargetView Failed.");

		//D3D11_TEXTURE2D_DESC depthStencilDesc;
		CD3D11_TEXTURE2D_DESC depthStencilDesc{
			DXGI_FORMAT_D24_UNORM_S8_UINT,
			static_cast<UINT>(windowWidth),
			static_cast<UINT>(windowHeight)
		};
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		// depth stencil 버퍼 생성
		hr = this->device->CreateTexture2D(&depthStencilDesc, NULL, this->depthStencilBuffer.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "CreateTexture2D for depthStencilBuffer failed.");

		// depth stencil view 생성
		hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "CreateDepthStencilView failed.");

		// output merger 의 렌더 타겟 설정
		this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

		// depth stencil state 생성
		//D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
		CD3D11_DEPTH_STENCIL_DESC depthStencilStateDesc{ D3D11_DEFAULT };
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

		hr = this->device->CreateDepthStencilState(&depthStencilStateDesc, this->depthStencilState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "CreateDepthStencilState failed.");

		//D3D11_VIEWPORT viewport;
		CD3D11_VIEWPORT viewport{
			0.0f,
			0.0f,
			static_cast<float>(windowWidth),
			static_cast<float>(windowHeight)
		};

		// 뷰포트 설정
		this->deviceContext->RSSetViewports(1, &viewport);

		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));

		rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

		// rasterizer state 설정
		hr = this->device->CreateRasterizerState(&rasterizerDesc, raterizerState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "CreateRasterizerState failed.");

		// 블랜드 스테이트 설정 
		D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc;
		ZeroMemory(&renderTargetBlendDesc, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));

		renderTargetBlendDesc.BlendEnable = true;
		renderTargetBlendDesc.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		renderTargetBlendDesc.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		renderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

		D3D11_BLEND_DESC blendDesc;
		ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

		blendDesc.RenderTarget[0] = renderTargetBlendDesc;

		hr = this->device->CreateBlendState(&blendDesc, this->blendState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "CreateBlendState failed.");

		// 스프라이트 폰트 설정
		spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
		spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Data\\Fonts\\comic_sans_ms_16.spritefont");

		// 텍스처 : 샘플러 스테이트 설정
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));

		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		hr = this->device->CreateSamplerState(&sampDesc, this->samplerState.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "CreateSamplerState failed.");
	}
	catch (COMException& exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}

	return true;
}

bool Graphics::InitializeShaders()
{
	std::wstring shaderfolder;
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
		{"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT numElements = ARRAYSIZE(layout);

	if (!vertexShader.Initialize(this->device, shaderfolder + L"vertexShader.cso", layout, numElements))
	{
		return false;
	}

	if (!pixelShader.Initialize(this->device, shaderfolder + L"pixelShader.cso"))
	{
		return false;
	}

	return true;
}

bool Graphics::InitializeScene()
{
	try
	{
		HRESULT hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\Ivan_1304_DIFF.jpg", nullptr, Texture_1.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "CreateWICTextureFromFIle failed.");

		hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Data\\Textures\\testTexture2.png", nullptr, Texture_2.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "CreateWICTextureFromFIle failed.");

		hr = cb_vs_vertexShader.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "vertex constant buffer initialization failed.");

		hr = cb_ps_pixelShader.Initialize(this->device.Get(), this->deviceContext.Get());
		COM_ERROR_IF_FAILED(hr, "pixel constant buffer initialization failed.");

		// 모델 초기화
		if (!gameObject.Initialize("Data\\Objects\\car.fbx", this->device.Get(), this->deviceContext.Get(), this->cb_vs_vertexShader))
		{
			return false;
		}

		camera.SetPosition(0.0f, 0.0f, -2.0f);
		camera.SetProjectionValues(90.0f, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 1000.0f);
	}
	catch (COMException& exception)
	{
		ErrorLogger::Log(exception);
		return false;
	}

	return true;
}

// input assembler
// vertex shader
// rasterizer
// pixel shader
// ouput merger
