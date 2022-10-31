#pragma once

#include "TEngineDefines.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

class TAdapterData
{
private:
	IDXGIAdapter* m_IDXGIAdapter = nullptr;
	DXGI_ADAPTER_DESC m_AdapterDescription;

public:
	TAdapterData(IDXGIAdapter* pAdapter);

	IDXGIAdapter* GetIDXGIAdapter() const;
	const DXGI_ADAPTER_DESC& GetAdapterDescription() const;
};

class TAdapterManager
{
public:
	bool Initialize();

	TAdapterManager() = default;
	TAdapterManager(const TAdapterManager& InOhter) = delete;
	TAdapterManager(const TAdapterManager&& InOther) = delete;
	TAdapterManager& operator=(const TAdapterManager& InOhter) = delete;
	TAdapterManager&& operator=(const TAdapterManager&& InOther) = delete;

	const std::vector<TAdapterData>& GetAdapters();
	TAdapterData* GetSelectedAdpaterData();
private:
	std::vector<TAdapterData> m_Adapters;
	int m_SeclectedAdapterDataIndex = INVALID_INDEX;

	bool SelectAdapter();
};