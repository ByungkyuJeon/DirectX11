#define STATIC_ENGINE_IMPORT 1

#include "EngineImports.h"
#include "TGame.h"
#include <stdio.h>

static int GuardedMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow);

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	try
	{
		// guarding main make able to handle exception
		GuardedMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	}
	catch(int exception)
	{
		// handle main exception
	}

	return 0;
}

int GuardedMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// 1. Initialize Engine
	InitEngine(hInstance);

	HRESULT hResult = CoInitialize(NULL);
	if (FAILED(hResult))
	{
		PRINT_ERROR("CoInitialize failed.");
		return -1;
	}

	while (gEngine->EngineTick())
	{

	}

	return 0;
}