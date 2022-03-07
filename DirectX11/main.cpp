#include "GameEngine.h"

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	GameEngine engine;
	engine.Initialize(hInstance, "Title", "WindowClass", 800, 600);

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