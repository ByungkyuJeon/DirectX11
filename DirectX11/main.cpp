#include "Engine.h"
#pragma comment(lib, "d3d11.lib")
#pragma commnet(lib, "DirectXTK.lib")

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	Engine engine;
	engine.Initialize(hInstance, "Title", "WindowClass", 800, 600);

	while (engine.ProcessMessages())
	{
		engine.Update();
		engine.RenderFrame();
	}

	return 0;
}