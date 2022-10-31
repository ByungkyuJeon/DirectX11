#pragma once

#include "TEngineCore.h"
#include "TMemoryPool.h"
#include "TWindow.h"
#include "TRenderer.h"
#include "TPhysics.h"
#include "Map.h"
#include "ModelManager.h"
#include "MapManager.h"

class TEngine
{
private:

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

	bool EngineTick();
};

extern TEngine* gEngine;

extern bool BootEngine(HINSTANCE InHInstance);