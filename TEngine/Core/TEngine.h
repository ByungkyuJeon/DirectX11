#pragma once

#include "Core.h"
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
	
};

class TEngine
{
private:
	HWND m_HWND;
	HINSTANCE m_HInstance;
	TEngineConfig m_EngineConfig;

	virtual bool InternalBootstrap(int InWindowWidth, int InWindowHeight);

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

	HINSTANCE GetHInstance() const;

	virtual bool Initiate(HWND InHWND, HINSTANCE InHInstance, int InWindowWidth, int InWindowHeight);
};

static TEngine* ExEngine;

extern "C" DLL_EXPORTS TEngine* ExBootEngine(HWND InHWND, HINSTANCE InHInstance, int InWindowWidth, int InWindowHeight);