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

	return true;
}

void Graphics::RenderFrame()
{
	float bgcolor[] = { 0.0f, 1.0f, 1.0f, 1.0f };
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), bgcolor);
	// vsync on : 1
	// vsync off : 0
	this->swapChain->Present(1, NULL);
}

bool Graphics::InitializeDirectX(HWND hwnd, int width, int height)
{
	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();
	int selectedAdapterIdx = 0;

	// �νĵ� ��� üũ
	if (adapters.size() < 1)
	{
		ErrorLogger::Log("No IDXGI Adapaters found.");
		return false;
	}
	else
	{
		// ���� �޸𸮰� ���� ū ��� ����
		for (int adapterIdx = 0; adapterIdx < adapters.size(); adapterIdx++)
		{
			if (adapters[adapterIdx].description.DedicatedVideoMemory > adapters[selectedAdapterIdx].description.DedicatedVideoMemory)
			{
				selectedAdapterIdx = adapterIdx;
			}
		}
	}

	// ���� ü�� ����ü ����
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
	// 1 - ���� ���۸�
	// 2 - Ʈ���� ���۸�
	scd.BufferCount = 1;
	scd.OutputWindow = hwnd;
	scd.Windowed = true;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// ���� ü�� ������Ʈ ����
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

	// ���� ü�� ����� �ּ� �о����
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed GetBuffer.");
		return false;
	}

	// ���� Ÿ�� �� ����
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

// input assembler
// vertex shader
// rasterizer
// pixel shader
// ouput merger
