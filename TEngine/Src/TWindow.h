#pragma once

#include "TEngineDefines.h"
#include "KeyboardClass.h"
#include "MouseClass.h"
#include <Windows.h>
#include <string>

class TWindow
{
public:
	TWindow();
	~TWindow();

	bool Initialize();
	bool ProcessMessages();

	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	KeyboardClass& GetKeyboardClass() { return m_KeyboardClass; };
	MouseClass& GetMouseClass() { return m_MouseClass; };

private:
	// input devices
	KeyboardClass m_KeyboardClass;
	MouseClass m_MouseClass;

	void RegisterWindowClass();
	void UnRegisterWindowClass();
};