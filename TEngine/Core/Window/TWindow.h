#pragma once

#include "../DevelopmentDefines.h"
#include <Windows.h>
#include <string>

struct TWindowData
{
#pragma message(TODO "�̰Ŵ� Ŭ�� ���� ���͸����� �ε��ؿ;� �Ѵ�.")
	std::string window_title = "";
	std::string window_class = "";
	int width = 0;
	int height = 0;
};

class TWindow
{
public:
	bool Initialize(HINSTANCE InHInstance);


private:
	HWND m_Handle = NULL;
	HINSTANCE m_HInstance = NULL;

#pragma message(TODO "������ �ε� ���� ���� ��.")
	void LoadData();
};