#pragma once

#include<d3d11.h>
#include <wrl/client.h>
#include <vector>

class AdapterData
{
public:
	AdapterData(IDXGIAdapter* pAdapter);
	IDXGIAdapter* pAdapter = nullptr;
	DXGI_ADAPTER_DESC description;
};

class AdapterReader
{
public:
	static const std::vector<AdapterData>& GetAdapters();
private:
	static std::vector<AdapterData> adapters;
};