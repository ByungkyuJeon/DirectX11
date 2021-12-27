#include "Graphics.h"

bool Graphics::Initialize(HWND hwnd, int width, int height)
{
	if (!InitializeDirectX(hwnd, width, height))
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

	this->deviceContext->IASetInputLayout(this->vertexShader.GetInputLayout());
	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//D3D10_PRIMITIVE_TOPOLOGY_POINTLIST
	//D3D10_PRIMITIVE_TOPOLOGY_LINELIST
	//D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST

	this->deviceContext->VSSetShader(this->vertexShader.GetShader(), NULL, 0);
	this->deviceContext->PSSetShader(this->pixelShader.GetShader(), NULL, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

	this->deviceContext->Draw(3, 0);

	// vsync on : 1
	// vsync off : 0
	this->swapChain->Present(1, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd, int width, int height)
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

	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
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

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create device and swapchain.");
		return false;
	}

	// 스왑 체인 백버퍼 주소 읽어오기
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed GetBuffer.");
		return false;
	}

	// 렌더 타겟 뷰 생성
	hr = this->device->CreateRenderTargetView(backBuffer.Get() , NULL, this->renderTargetView.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "CreateRenderTargetView Failed.");
		return false;
	}

	this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;

	this->deviceContext->RSSetViewports(1, &viewport);

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

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 }, // Offset macro : D3D11_APPEND_ALIGNED_ELEMENT
		{"COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0}
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
	Vertex v[] =
	{
		Vertex{-0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
		Vertex{0.0f, 0.5f, 0.0f, 1.0f, 0.0f},
		Vertex{0.5f, -0.5f, 0.0f, 0.0f, 1.0f},
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(v);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));

	vertexBufferData.pSysMem = v;

	HRESULT hr = this->device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, this->vertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "InitializeScene failed.");
		return false;
	}

	return true;
}

// input assembler
// vertex shader
// rasterizer
// pixel shader
// ouput merger
