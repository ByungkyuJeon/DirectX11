#pragma once

// imported libraries
#pragma comment(lib, "TEngine.lib")

//#ifdef CREATE_DLL_EXPORTS
//#define DLL_EXPORTS __declspec(dllexport)
//#else
//#define DLL_EXPORTS __declspec(dllimport)
//#endif

// imported headers
#include "TEngine.h"

//HMODULE EngineModule;

//typedef TEngine* (*fnBootEngine)(HINSTANCE);
//fnBootEngine BootEngine = (fnBootEngine)GetProcAddress((EngineModule = LoadLibrary(L"TEngine.dll")), "ExBootEngine");
//
//extern TEngine* ExEngine;

bool InitEngine(HINSTANCE InHInstance)
{
	/*if (!EngineModule)
		return false;*/

	ExEngine = ExBootEngine(InHInstance);
	return 1;
}