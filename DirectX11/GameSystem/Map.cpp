#include "Map.h"

void Map::update(float delta)
{
	for (auto& elem : this->mGameObjects)
	{
		elem->update(delta);
	}
}

void Map::addGameObject(std::shared_ptr<GameObject> gameObject)
{
	this->mGameObjects.emplace_back(gameObject);
}
