#include "Engine.h"
#pragma comment(lib, "d3d11.lib")
#pragma commnet(lib, "DirectXTK.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	// 메시지 처리
	switch (message)
	{
	case WM_PAINT:

		hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);

		break;
	case WM_DESTROY:

		PostQuitMessage(0);

		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	Engine engine;
	engine.Initialize(hInstance, "Title", "WindowClass", 1000, 1000);

	while (engine.ProcessMessages())
	{

	}

	return 0;
}