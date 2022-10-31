#pragma once

#include <Windows.h>
#include <string>

#ifndef RELEASE
#define _LOG_STAT
class TStat
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

#if !RELEASE
#define PRINT_STAT(msg) TStat::PrintStatLog(std::string(__FILE__) + "(" + std::to_string(__LINE__) + ") : " + msg);
#else
#define PRINT_STAT(msg) 
#endif

