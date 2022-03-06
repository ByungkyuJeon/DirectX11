#pragma once

#include "../CoordinateSystem/Transform.h"
#include "../Graphics/Model.h"

struct GameObjectSetting
{
	GameObjectSetting() : isHiddenInGame{ false } {}
	GameObjectSetting(bool isHiddenInGame) : isHiddenInGame{ isHiddenInGame } {}

	bool isHiddenInGame;
};

class GameObject
{
public:

	void setTransform(std::shared_ptr<Transform> transform);
	void setModel(std::shared_ptr<Model> model);

	std::shared_ptr<Transform> getTransform() const;
	std::shared_ptr<Model> getModel() const;


protected:
	virtual void update();
	virtual void updateMatrix();

	GameObjectSetting gameObjectSetting;

	std::shared_ptr<Transform> mTransform;
	std::shared_ptr<Model> mModel;
};