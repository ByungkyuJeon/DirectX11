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
