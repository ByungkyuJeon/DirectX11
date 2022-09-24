#include "EngineImports.h"
#include "TGame.h"
#include <stdio.h>

int GuardedMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow);

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	GuardedMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

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

	while (ExEngine->EngineTick())
	{

	}

	return 0;
}