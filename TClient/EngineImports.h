#pragma once

#if STATIC_ENGINE_IMPORT 
// imported libraries
#pragma comment(lib, "TEngine.lib")
#else
#ifdef CREATE_DLL_EXPORTS
#define DLL_EXPORTS __declspec(dllexport)
#else
#define DLL_EXPORTS __declspec(dllimport)
#endif
#endif

// imported headers
#include "TEngine.h"

bool InitEngine(HINSTANCE InHInstance)
{
	Engine = BootEngine(InHInstance);
	if (Engine == nullptr)
		return false;

	return true;
}