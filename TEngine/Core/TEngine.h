#pragma once

#include "Graphics/Renderer.h"

struct TEngineConfig
{

};

class TEngine
{
private:
	// �޸� Ǯ (+GC?)


	// �׷��Ƚ� ������
	Renderer renderer;


	// 

public:

	

};

static const TEngine* Engine;

static void BootEngine()
{
	Engine = new TEngine();
}