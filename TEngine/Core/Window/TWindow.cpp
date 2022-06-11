#include "TWindow.h"

bool TWindow::Initialize(int InWindowWidth, int InWindowHeight)
{
    this->m_WindowData.SetWidth(InWindowHeight);
    this->m_WindowData.SetHeight(InWindowHeight);

    return true;
}

int TWindowSettingData::GetWidth() const
{
    return m_Width;
}

int TWindowSettingData::GetHeight() const
{
    return m_Height;
}

void TWindowSettingData::SetWidth(int InWindowWidth)
{
    this->m_Width = InWindowWidth;
}

void TWindowSettingData::SetHeight(int InWindowHeight)
{
    this->m_Height = InWindowHeight;
}
