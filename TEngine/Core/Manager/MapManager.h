#pragma once

#include "../Framework/Game/Map.h"
#include <string>
#include <unordered_map>

class MapManager
{
public:
	MapManager() {};

	bool initialize();

	void loadMaps();

private:

	Map loadMap(const std::string& directory);

	// game maps (hash table based)
	std::unordered_map<std::string, Map> maps;
};