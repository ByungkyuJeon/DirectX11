#pragma once

#include <Windows.h>

class TRenderer
{
private:
	HWND m_HWND;
	int m_SeclectedAdapterDataIndex = INVALID_INDEX;

	bool InitializeDirectX();
	bool InitializeShaders();
	bool InitializeScene();



public:
	bool Initiate(HWND InHWND);
};