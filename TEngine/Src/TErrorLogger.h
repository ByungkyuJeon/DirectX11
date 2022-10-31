#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif // !NOMINMAX

#include <Windows.h>
#include <string>
#include "ErrorCode.h"

class TErrorLogger
{
public:
	static void PrintErrorLog(const std::string& InMsg)
	{
		static const std::string messagePrefix = "ERROR LOG : ";

		std::string errorMsg = messagePrefix + InMsg + '\n';
		OutputDebugStringA(errorMsg.c_str());
	}

	static void PrintErrorLog(ErrorCode InErrorCode)
	{
		static const std::string messagePrefix = "ERROR LOG : ";

		std::string errorMsg = messagePrefix + ErrorCodeToString(InErrorCode) + '\n';
		OutputDebugStringA(errorMsg.c_str());
	}

private:

};

#if !_RELEASE
#define PRINT_ERROR(msg) TErrorLogger::PrintErrorLog(std::string(__FILE__) + "(" + std::to_string(__LINE__) + ") : " + msg);
#else
#define PRINT_ERROR(msg) 
#endif