#include "EngineImports.h"
#include "GameEngine.h"
#include <stdio.h>



int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// 1. Initialize Engine dll
	InitEngine(hInstance);
	//BootEngine(hInstance);

	GameEngine engine;
	engine.Initialize(hInstance, "Title", "WindowClass", 1920, 1080);

	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "CoInitialize failed.");
		return -1;
	}

	while (engine.ProcessMessages())
	{
		engine.Update();
		engine.RenderFrame();
	}

	return 0;
}