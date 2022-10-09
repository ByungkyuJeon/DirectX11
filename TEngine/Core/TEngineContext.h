#pragma once

#include "TEngineCore.h"

class TEngineContext
{
private:

public:
	bool Initialize();

private:


};

extern TEngineContext* GContext;

TEngineContext* InitializeGContext()
{
	if (GContext != nullptr)
	{
		PRINT_ERROR("engine context already initialized.");
		return nullptr;
	}

	GContext = new TEngineContext();

	if (false == GContext->Initialize())
	{
		PRINT_ERROR("engine context initialization failed.");
		return nullptr;
	}

	return GContext;
}