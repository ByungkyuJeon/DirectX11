#include "TAdapterManager.h"
#include "../Framework/Logger/ErrorLogger/TErrorLogger.h"

TAdapterData::TAdapterData(IDXGIAdapter* pAdapter)
{
	this->m_IDXGIAdapter = pAdapter;
	HRESULT hr = pAdapter->GetDesc(&this->m_AdapterDescription);
	if (FAILED(hr))
	{
		PRINT_ERROR("Failed to Get Description for IDXGIAdapter.")
	}
}

IDXGIAdapter* TAdapterData::GetIDXGIAdapter() const
{
	return m_IDXGIAdapter;
}

const DXGI_ADAPTER_DESC& TAdapterData::GetAdapterDescription() const
{
	return m_AdapterDescription;
}

bool TAdapterManager::Initialize()
{
	Microsoft::WRL::ComPtr<IDXGIFactory> pFactory;

	// DXGIFactory object ����
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(pFactory.GetAddressOf()));
	if (FAILED(hr))
	{
		PRINT_ERROR("Failed to create DXGIFactory for enumerating adapters");
		return false;
	}

	IDXGIAdapter* pAdapter;
	UINT index = 0;
	while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter)))
	{
		m_Adapters.emplace_back(TAdapterData(pAdapter));
		index++;
	}

	if (!SelectAdapter())
		return false;

	return true;
}

const std::vector<TAdapterData>& TAdapterManager::GetAdapters()
{
	return m_Adapters;
}

TAdapterData* TAdapterManager::GetSelectedAdpaterData()
{
	return &m_Adapters[m_SeclectedAdapterDataIndex];
}

bool TAdapterManager::SelectAdapter()
{
	int selectedAdapterIdx = 0;

	// ���� �޸𸮰� ���� ū ��� ����
	for (int adapterIdx = 0; adapterIdx < m_Adapters.size(); adapterIdx++)
	{
		if (m_Adapters[adapterIdx].GetAdapterDescription().DedicatedVideoMemory > m_Adapters[selectedAdapterIdx].GetAdapterDescription().DedicatedVideoMemory)
		{
			m_SeclectedAdapterDataIndex = adapterIdx;
		}
	}

	// ��� ��ȿ�� üũ
	if (m_Adapters.size() < 1 || m_SeclectedAdapterDataIndex == INVALID_INDEX)
		return false;

	return true;
}
