#pragma once

#include "Framework/Memory/TMemoryPool.h"
#include "Graphics/Renderer.h"
#include "Physics/Physics.h"

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

	bool InternalBootstrap();

	// 메모리 풀

	// GC

	// 그래픽스 렌더러
	Renderer m_Renderer;

	// 
	Physics m_Physics;

public:
	TEngine() {};
	TEngine(const TEngine& InOther) = delete;
	TEngine(const TEngine&& InOther) = delete;
	TEngine& operator=(const TEngine& InOther) = delete;
	TEngine& operator=(const TEngine&& InOther) = delete;

	bool Initiate(HINSTANCE InHInstance);
};

static TEngine Engine;

static bool BootEngine(HINSTANCE InHInstance)
{
	PRINT_STAT("---Engine Bootstrap Started---");

	// for now, alloc at static
	//Engine = new TEngine();
	if (!Engine.Initiate(InHInstance))
	{
		PRINT_STAT("---Engine Bootstrap Failed---");
		return false;
	}

	PRINT_STAT("---Engine Bootstrap Succeed---");

	return true;
}