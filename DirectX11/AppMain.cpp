#include "EngineImports.h"
#include <stdio.h>



int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// 1. Initialize Engine dll
	InitEngine(hInstance);
	//BootEngine(hInstance);

	HRESULT hResult = CoInitialize(NULL);
	if (FAILED(hResult))
	{
		PRINT_ERROR("CoInitialize failed.");
		return -1;
	}

	while (Engine->EngineTick())
	{
		
	}

	return 0;
}