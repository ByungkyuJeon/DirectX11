#include "ErrorLogger.h"
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 1. 윈도우 구조체 할당
	WNDCLASSEX wcex;
	//wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = 0;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	//wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"base";

	// 1.1 윈도우 구조체 등록
	RegisterClassEx(&wcex);

	// 2. 윈도우 생성
	HWND hWindow = CreateWindow(
		L"base",
		L"DX11Prac",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1000, 1000, NULL, NULL,
		hInstance, NULL
	);

	// 3. 윈도우 출력
	ShowWindow(hWindow, SW_SHOW);
	UpdateWindow(hWindow);

	// 4. 메시지 루프
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}