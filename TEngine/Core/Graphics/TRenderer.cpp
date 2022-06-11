#include "TRenderer.h"
#include "../Core.h"
#include "../Framework/Logger/ErrorLogger/TErrorLogger.h"
#include "TAdapterManager.h"

bool TRenderer::InitializeDirectX()
{
	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

	// ���� �޸𸮰� ���� ū ��� ����
	for (int adapterIdx = 0; adapterIdx < adapters.size(); adapterIdx++)
	{
		if (adapters[adapterIdx].description.DedicatedVideoMemory > adapters[selectedAdapterIdx].description.DedicatedVideoMemory)
		{
			m_SeclectedAdapterDataIndex = adapterIdx;
		}
	}

	// ��� ��ȿ�� üũ
	if (adapters.size() < 1 || m_SeclectedAdapterDataIndex == INVALID_INDEX)
	{
		PRINT_ERROR("No IDXGI Adapaters found.")
		return false;
	}

    return true;
}

bool TRenderer::Initiate(HWND InHWND)
{
    m_HWND = InHWND;

    if (!InitializeDirectX())
    {
        PRINT_ERROR("DirectX initialization failed.")
        return false;
    }

    return true;
}
