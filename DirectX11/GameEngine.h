#pragma once

#include "WindowContainer.h"
#include "Graphics/GraphicsEngine.h"
#include "Physics/PhysicsEngine.h"
#include "Factory/ModelFactory.h"
#include "FrameTimer.h"
#include "GameSystem/Map.h"
#include <unordered_map>

/// <summary>
/// the most top level game engine
/// </summary>
class GameEngine : private WindowContainer
{
public:
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
	bool ProcessMessages();

	void Update();
	void RenderFrame();

	void LoadMap(const std::string& mapName);
	void UnloadMap(const std::string& mapName);

	std::string currentMap;
	
private:
	// frame timer
	FrameTimer frameTimer;

	// graphics module
	GraphicsEngine mGraphics;
	
	// physics module
	PhysicsEngine mPhysicsEngine;

	ModelFactory mModelFactory;

	// game maps (hash table based)
	std::unordered_map<std::string, Map> maps;
};