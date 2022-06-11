#pragma once

#include "../DevelopmentDefines.h"
#include <Windows.h>
#include <string>

struct TWindowSettingData
{
#pragma message(TODO "이거는 런처로부터 받자.")
	int m_Width = 0;
	int m_Height = 0;

	int GetWidth() const;
	int GetHeight() const;
	void SetWidth(int InWindowWidth);
	void SetHeight(int InWindowHeight);

};

class TWindow
{
public:
	bool Initialize(int InWindowWidth, int InWindowHeight);

private:
	TWindowSettingData m_WindowData;
};