#pragma once

#include <Windows.h>

#if !_RELEASE
#define _LOG_STAT
class TStatLogger
{
public:
	static void PrintStatLog(const std::string& InMsg)
	{
		static const std::string messagePrefix = "STAT LOG : ";
		
		std::string errorMsg = messagePrefix + InMsg + '\n';
		OutputDebugStringA(errorMsg.c_str());
	}

private:

};
#endif

#if !_RELEASE
#define PRINT_STAT(msg) TStatLogger::PrintStatLog(std::string(__FILE__) + "(" + std::to_string(__LINE__) + ") : " + msg);
#else
#define PRINT_STAT(msg) 
#endif

