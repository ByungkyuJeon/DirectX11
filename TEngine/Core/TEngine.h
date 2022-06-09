#pragma once

#include "Framework/Memory/TMemoryPool.h"
#include "Window/TWindow.h"
#include "Graphics/TRenderer.h"
#include "Physics/TPhysics.h"

#ifdef CREATE_DLL_EXPORTS
#define DLL_EXPORTS __declspec(dllexport)
#else
#define DLL_EXPORTS __declspec(dllimport)
#endif

struct TEngineConfig
{
	std::string m_WindowTitle;
	std::string m_WindowClass;
	int m_InitWidth;
	int m_InitHeight;
};

class TEngine
{
private:
	HINSTANCE m_HInstance;
	TEngineConfig m_EngineConfig;

	virtual bool InternalBootstrap();

	// 윈도우
	TWindow m_Window;

	// DirectX
	

	// 메모리 풀

	// GC

	// 그래픽스 렌더러
	TRenderer m_Renderer;

	// 
	TPhysics m_Physics;

public:
	TEngine() {};
	TEngine(const TEngine& InOther) = delete;
	TEngine(const TEngine&& InOther) = delete;
	TEngine& operator=(const TEngine& InOther) = delete;
	TEngine& operator=(const TEngine&& InOther) = delete;

	virtual bool Initiate(HINSTANCE InHInstance);
};

static TEngine* ExEngine;

extern "C" DLL_EXPORTS TEngine* ExBootEngine(HINSTANCE InHInstance);