#pragma once

#include "WindowContainer.h"
#include "Graphics/Graphics.h"
#include "FrameTimer.h"
#include "GameSystem/Map.h"
#include "Physics/PhysicsEngine.h"
#include <unordered_map>



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

	void LoadMap(const std::string& mapName);
	void UnloadMap(const std::string& mapName);



private:
	// frame timer
	FrameTimer frameTimer;

	// graphics module
	Graphics graphics;

	// physics module
	PhysicsEngine physicsEngine;

	// game maps (hash table based)
	std::unordered_map<std::string, Map> maps;


	
};