#pragma once

#include <string>

#ifndef ENGINE_DEFINE
#define ENGINE_DEFINE

#if DEBUG
#define TO_STR(msg) #msg
#define FORMAT(line) __FILE__ "(" TO_STR(line) "): TODO - "
#define TODO FORMAT(__LINE__)
#else
#define TODO 
#endif

// INVALID VALUES
#define INVALID_HANDLE 0
#define INVALID_LENGTH -1
#define INVALID_INDEX -1
// INVALID VALUES

// TYPE DEFINES
#define TComPtr Microsoft::WRL::ComPtr
typedef std::wstring TWString;
typedef std::string TString;
typedef unsigned int TLength;

#endif // !ENGINE_DEFINE