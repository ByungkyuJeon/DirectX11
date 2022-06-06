#pragma once

#if !_RELEASE
#include <Windows.h>
#define _LOG_STAT
class TStatLogger
{
public:
	static void PrintStatLog(const std::string& msg)
	{
		static const std::string m_MessagePrefix = "STAT LOGGER : ";
		
		std::string errorMsg = m_MessagePrefix + msg + '\n';
		OutputDebugStringA(errorMsg.c_str());
	}

private:

};
#endif

#if !_RELEASE
#define PRINT_STAT(msg) TStatLogger::PrintStatLog(msg);
#else
#define PRINT_STAT(msg) 
#endif

