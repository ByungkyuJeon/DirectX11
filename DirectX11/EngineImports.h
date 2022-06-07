#pragma once

// imported libraries
//#pragma comment(lib, "TEngine.lib")

#ifdef CREATE_DLL_EXPORTS
#define DLL_EXPORTS __declspec(dllexport)
#else
#define DLL_EXPORTS __declspec(dllimport)
#endif

// imported headers
#include <TEngine.h>

HMODULE EngineModule;

static TEngine* Engine;

typedef TEngine* (*fnBootEngine)(HINSTANCE);
fnBootEngine BootEngine = (fnBootEngine)GetProcAddress((EngineModule = LoadLibrary(L"TEngine.dll")), "ExBootEngine");

bool InitEngine(HINSTANCE InHInstance)
{
	if (!EngineModule)
		return false;

	Engine = BootEngine(InHInstance);
	return 1;
}