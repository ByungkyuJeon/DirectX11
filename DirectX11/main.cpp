#include "ErrorLogger.h"
#pragma comment(lib, "d3d11.lib")
#pragma commnet(lib, "DirectXTK.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	// �޽��� ó��
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
	// 1. ������ ����ü �Ҵ�
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

	// 1.1 ������ ����ü ���
	RegisterClassEx(&wcex);

	// 2. ������ ����
	HWND hWindow = CreateWindow(
		L"base",
		L"DX11Prac",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1000, 1000, NULL, NULL,
		hInstance, NULL
	);

	// 3. ������ ���
	ShowWindow(hWindow, SW_SHOW);
	UpdateWindow(hWindow);

	// 4. �޽��� ����
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}