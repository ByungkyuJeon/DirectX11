#pragma once

#include "Framework/Memory/TMemoryPool.h"
#include "Graphics/Renderer.h"


struct TEngineConfig
{

};

class TEngine
{
private:
	bool InternalBootstrap();

	// 메모리 풀

	// GC

	// 그래픽스 렌더러
	Renderer renderer;


	// 

public:
	TEngine();

	bool Initiate();
	

};

static const TEngine* Engine;

static void BootEngine()
{
	Engine = new TEngine();

	
}