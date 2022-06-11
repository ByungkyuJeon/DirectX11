#define CREATE_DLL_EXPORTS
#include "TEngine.h"
#include "Framework/Logger/ErrorLogger/TErrorLogger.h"

bool TEngine::InternalBootstrap(int InWindowWidth, int InWindowHeight)
{
	if (!m_Window.Initialize(InWindowWidth, InWindowHeight))
	{
		PRINT_ERROR("Window Initialization failed.")
		return false;
	}

	if (!m_Renderer.Initiate(m_HWND))
	{
		PRINT_ERROR("Renderer Initialization failed.")
		return false;
	}

	if (!m_Physics.Initiate())
	{
		PRINT_ERROR("Physics Initialization failed.")
			return false;
	}


	return true;
}

HINSTANCE TEngine::GetHInstance() const
{
	return m_HInstance;
}

bool TEngine::Initiate(HWND InHWND, HINSTANCE InHInstance, int InWindowWidth, int InWindowHeight)
{
	m_HWND = InHWND;
	m_HInstance = InHInstance;

	try
	{
		InternalBootstrap(InWindowWidth, InWindowHeight);
	}
	catch (ErrorCode errCode)
	{
		std::string errorStr = ErrorCodeToString(errCode);
		OutputDebugStringA(errorStr.c_str());
		return false;
	}

	return true;
}

DLL_EXPORTS TEngine* ExBootEngine(HWND InHWND, HINSTANCE InHInstance, int InWindowWidth, int InWindowHeight)
{
	PRINT_STAT("---Engine Bootstrap Started---");

	ExEngine = new TEngine();
	if (!ExEngine->Initiate(InHWND, InHInstance, InWindowWidth, InWindowHeight))
	{
		PRINT_STAT("---Engine Bootstrap Failed---");
		return nullptr;
	}

	PRINT_STAT("---Engine Bootstrap Succeed---");

	return ExEngine;
}
