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

	// �޸� Ǯ

	// GC

	// �׷��Ƚ� ������
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