#pragma once

#include "../Core.h"
#include "../DevelopmentDefines.h"
#include "../Framework/Input/Keyboard/KeyboardClass.h"
#include "../Framework/Input/Mouse/MouseClass.h"
#include <Windows.h>
#include <string>

struct TWindowData
{
#pragma message(TODO "이거는 런처로부터 받자.")
	TString m_WindowTitle;
	TWString m_WideWindowTitle;
	TString m_WindowClass;
	TWString m_WideWindowClass;
	int m_Width = 0;
	int m_Height = 0;

	const TString& GetWindowTitle() const;
	const TWString& GetWideWindowTitle() const;
	const TString& GetWindowClass() const;
	const TWString& GetWideWindowClass() const;
	int GetWidth() const;
	int GetHeight() const;
	void SetWidth(int InWindowWidth);
	void SetHeight(int InWindowHeight);
};

class TWindow
{
public:
	~TWindow();

	bool Initialize();
	bool ProcessMessages();

	LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	const TWindowData& GetWindowData() const;
	KeyboardClass& GetKeyboardClass() { return m_KeyboardClass; };
	MouseClass& GetMouseClass() { return m_MouseClass; };

private:
	TWindowData m_WindowData;

	// input devices
	KeyboardClass m_KeyboardClass;
	MouseClass m_MouseClass;

	void RegisterWindowClass();
};