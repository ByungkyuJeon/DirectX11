#pragma once

#include "TEngineCore.h"
#include "TEngineConfigManager.h"

class TEngineContext
{
private:


public:
	bool Initialize();
	~TEngineContext();

	// Managers
	TEngineConfigManager* gEngineConfigManager = nullptr;

	// DataTables


private:

};

extern TEngineContext* gContext;

static bool InitializeContext()
{
	if (gContext != nullptr)
	{
		PRINT_ERROR("engine context already initialized.");
		return false;
	}

	gContext = new TEngineContext();

	if (false == gContext->Initialize())
	{
		PRINT_ERROR("engine context initialization failed.");
		return false;
	}

	return true;
}