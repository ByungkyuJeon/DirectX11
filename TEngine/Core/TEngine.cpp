#define CREATE_DLL_EXPORTS
#include "TEngine.h"

bool TEngine::InternalBootstrap(int InWindowWidth, int InWindowHeight)
{
	if (!m_Window.Initialize(InWindowWidth, InWindowHeight))
	{
#pragma message(TODO "크래시 내고 에러 로그 작업 추가")
		return false;
	}


	return true;
}

HINSTANCE TEngine::GetHInstance() const
{
	return m_HInstance;
}

bool TEngine::Initiate(HINSTANCE InHInstance, int InWindowWidth, int InWindowHeight)
{
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

DLL_EXPORTS TEngine* ExBootEngine(HINSTANCE InHInstance, int InWindowWidth, int InWindowHeight)
{
	PRINT_STAT("---Engine Bootstrap Started---");

	ExEngine = new TEngine();
	if (!ExEngine->Initiate(InHInstance, InWindowWidth, InWindowHeight))
	{
		PRINT_STAT("---Engine Bootstrap Failed---");
		return nullptr;
	}

	PRINT_STAT("---Engine Bootstrap Succeed---");

	return ExEngine;
}
