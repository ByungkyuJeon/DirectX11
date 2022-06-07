#define CREATE_DLL_EXPORTS
#include "TEngine.h"

bool TEngine::InternalBootstrap()
{



	return true;
}

bool TEngine::Initiate(HINSTANCE InHInstance)
{
	m_HInstance = InHInstance;

	try
	{
		InternalBootstrap();
	}
	catch (ErrorCode errCode)
	{
		std::string errorStr = ErrorCodeToString(errCode);
		OutputDebugStringA(errorStr.c_str());
		return false;
	}

	return true;
}

DLL_EXPORTS TEngine* ExBootEngine(HINSTANCE InHInstance)
{
	PRINT_STAT("---Engine Bootstrap Started---");

	ExEngine = new TEngine();
	if (!ExEngine->Initiate(InHInstance))
	{
		PRINT_STAT("---Engine Bootstrap Failed---");
		return nullptr;
	}

	PRINT_STAT("---Engine Bootstrap Succeed---");

	return ExEngine;
}
