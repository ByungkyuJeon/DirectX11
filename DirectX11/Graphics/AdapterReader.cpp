#include "AdapterReader.h"

std::vector<AdapterData> AdapterReader::adapters;

AdapterData::AdapterData(IDXGIAdapter* pAdapter)
{
	this->pAdapter = pAdapter;
	HRESULT hr = pAdapter->GetDesc(&this->description);
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to Get Description for IDXGIAdapter.");
	}
}

std::vector<AdapterData> AdapterReader::GetAdapters()
{
	// 초기화 여부 체크
	if (adapters.size() > 0)
	{
		return adapters;
	}

	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;

	// DXGIFactory object 생성
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create DXGIFactory for enumerating adapters");
		exit(-1);
	}

	IDXGIAdapter* pAdapter;
	UINT index = 0;
	while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter)))
	{
		adapters.emplace_back(AdapterData(pAdapter));
		index++;
	}

	return adapters;
}
