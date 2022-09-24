#pragma once

#include "Core.h"
#include "Framework/Memory/TMemoryPool.h"
#include "Window/TWindow.h"
#include "Graphics/TRenderer.h"
#include "Physics/TPhysics.h"
#include "Framework/Game/Map.h"
#include "Manager/ModelManager.h"
#include "Manager/MapManager.h"

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

	TEngineConfig m_EngineConfig;

	bool InternalBootstrap();
	void Update();

	// 윈도우
	TWindow m_Window;

	// DirectX
	

	// 메모리 풀

	// GC

	// 그래픽스 렌더러
	TRenderer m_Renderer;

	// 
	TPhysics m_Physics;

	// model manager
	ModelManager m_ModelManager;

	// map manager
	MapManager m_MapManager;

	std::string currentMap;
	// game maps (hash table based)
	std::unordered_map<std::string, Map> maps;


	TFrameTimer frameTimer;
public:
	TEngine() {};
	TEngine(const TEngine& InOther) = delete;
	TEngine(const TEngine&& InOther) = delete;
	TEngine& operator=(const TEngine& InOther) = delete;
	TEngine& operator=(const TEngine&& InOther) = delete;

	bool Initialize(HINSTANCE InHInstance);

	virtual bool EngineTick();

	const TEngineConfig& GetConfig() const;
	Length GetWindowWidth() const;
	Length GetWindowHeight() const;
};

extern TEngine* ExEngine;

extern "C" DLL_EXPORTS TEngine* ExBootEngine(HINSTANCE InHInstance);