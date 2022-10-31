#include "TEngineCore.h"
#include "TWindowConfig.h"

TWindowConfig::TWindowConfig()
{

}

TWindowConfig::~TWindowConfig()
{
}

void TWindowConfig::Initialize()
{
	TDataElement engineConfigRootNode = GetRootDataNode("EngineConfig");
	if (false == engineConfigRootNode.IsValid())
	{
		PRINT_ERROR("engine config xml data read failed.");
		return;
	}

	TDataElement windowConfigNode = engineConfigRootNode.FirstChildElement("Window");
	if (windowConfigNode.IsValid())
	{
		m_IsFullScreen = windowConfigNode.FindBoolAttribute("isFullScreen");
		m_WindowWidth = windowConfigNode.FindIntAttribute("width");
		m_WindowHeight = windowConfigNode.FindIntAttribute("height");
		m_WindowTitle = windowConfigNode.FindStringAttribute("title");
	}
}

void TWindowConfig::Finalize()
{

}

const std::string& TWindowConfig::GetWindowTitle() const
{
	return m_WindowTitle;
}

const bool TWindowConfig::GetIsFullScreen() const
{
	return m_IsFullScreen;
}

const TLength TWindowConfig::GetWindowWidth() const
{
	return m_WindowWidth;
}

const TLength TWindowConfig::GetWindowHeight() const
{
	return m_WindowHeight;
}
