#pragma once

#include "WindowContainer.h"
#include "Graphics/Graphics.h"
#include "FrameTimer.h"
#include "GameSystem/Map.h"
#include "Physics/PhysicsEngine.h"

/// <summary>
/// the most top level game engine
/// </summary>
class Engine : private WindowContainer
{
public:
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessages();

	void Update();
	void RenderFrame();

private:
	FrameTimer frameTimer;

	// graphics module
	Graphics graphics;

	// physics module
	PhysicsEngine physicsEngine;

	// game maps
	std::vector<Map> maps;
};