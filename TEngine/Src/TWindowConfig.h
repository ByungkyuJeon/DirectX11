#pragma once

#include "TEngineDefines.h"

class TWindowConfig
{
private:
	std::string m_WindowTitle = "";
	bool m_IsFullScreen = true;
	TLength m_WindowWidth = INVALID_LENGTH;
	TLength m_WindowHeight = INVALID_LENGTH;

public:
	TWindowConfig();
	~TWindowConfig();

	void Initialize();
	void Finalize();
	void LoadData();

	const std::string& GetWindowTitle() const;
	const bool GetIsFullScreen() const;
	const TLength GetWindowWidth() const;
	const TLength GetWindowHeight() const;
};