#pragma once

#include "Graphics/Renderer.h"

struct TEngineConfig
{

};

class TEngine
{
private:
	// 메모리 풀 (+GC?)


	// 그래픽스 렌더러
	Renderer renderer;


	// 

public:

	

};

static const TEngine* Engine;

static void BootEngine()
{
	Engine = new TEngine();
}