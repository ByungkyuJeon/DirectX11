#include "TEngineContext.h"

TEngineContext* gContext;

bool TEngineContext::Initialize()
{
	if (false == InitializeDataIOWrapper())
	{
		PRINT_ERROR("data io wrrapper Initialization failed.");
		return false;
	}

	gEngineConfigManager = new TEngineConfigManager();
	gEngineConfigManager->Initialize();

	return true;
}

TEngineContext::~TEngineContext()
{
	gEngineConfigManager->Finalize();
	delete gEngineConfigManager;
	gEngineConfigManager = nullptr;
}