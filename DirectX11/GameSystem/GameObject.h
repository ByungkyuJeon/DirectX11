#pragma once

#include "../CoordinateSystem/Transform.h"
#include "../Graphics/Model.h"

struct GameObjectSetting
{
public:
	GameObjectSetting() : isHiddenInGame{ false } {}
	GameObjectSetting(bool isHiddenInGame) : isHiddenInGame{ isHiddenInGame } {}

	bool isHiddenInGame;
};

class GameObject
{
public:
	GameObject() {}
	GameObject(std::shared_ptr<Transform> transform);
	GameObject(std::shared_ptr<Transform> transform, std::shared_ptr<Model> model, std::shared_ptr<DirectX::XMFLOAT3> velocity);

	void setTransform(std::shared_ptr<Transform> transform);
	void setModel(std::shared_ptr<Model> model);
	void setVelocity(std::shared_ptr<DirectX::XMFLOAT3> velocity);
	
	std::shared_ptr<Transform> getTransform() const;
	std::shared_ptr<Model> getModel() const;
	std::shared_ptr<DirectX::XMFLOAT3> getVelocity() const;

	virtual void update(float delta);
protected:

	virtual void updateMatrix();

	GameObjectSetting gameObjectSetting;

	std::shared_ptr<Transform> mTransform;
	std::shared_ptr<Model> mModel;

	std::shared_ptr<DirectX::XMFLOAT3> mVelocity;
};