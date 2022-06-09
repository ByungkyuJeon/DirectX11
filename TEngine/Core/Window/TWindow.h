#pragma once

#include "../DevelopmentDefines.h"
#include <Windows.h>
#include <string>

struct TWindowData
{
#pragma message(TODO "이거는 클라 로컬 디렉터리에서 로드해와야 한다.")
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

#pragma message(TODO "데이터 로드 로직 만들 것.")
	void LoadData();
};