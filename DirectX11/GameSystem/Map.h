#pragma once

#include "GameObject.h"

class Map
{
public:
	void update(float delta);

	void addGameObject(std::shared_ptr<GameObject> gameObject);

private:

	std::vector<std::shared_ptr<GameObject>> mGameObjects;
};