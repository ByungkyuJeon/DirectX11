#pragma once

#include "../CoordinateSystem/Transform.h"
#include "../Graphics/Model.h"
#include "../Physics/Collider.h"

struct GameObjectSetting
{
public:
	GameObjectSetting() : mIsHiddenInGame{ false }, mCollisionDetectionEnabled{ false } {}
	GameObjectSetting(bool isHiddenInGame) : 
		mIsHiddenInGame{ isHiddenInGame }, mCollisionDetectionEnabled{ false }{}

	void setCollisionDetectionState(bool state);
	void setHiddenInGameState(bool state);

	bool IsHiddenInGame() const;
	bool CollisionDetectionEnabled() const;


private:
	bool mIsHiddenInGame;
	bool mCollisionDetectionEnabled;
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
	void setCollider(std::shared_ptr<Collider> collider);
	void setAcceleration(const DirectX::XMFLOAT3& acceleration);
	void setMass(float mass);
	
	const GameObjectSetting& getGameObjectSetting() const;
	std::shared_ptr<Transform> getTransform() const;
	std::shared_ptr<Model> getModel() const;
	std::shared_ptr<DirectX::XMFLOAT3> getVelocity() const;
	std::shared_ptr<Collider> getCollider() const;
	const DirectX::XMFLOAT3 getAcceleration() const;
	float getMass() const;

	void accelerate(const DirectX::XMFLOAT3& acceleration);

	virtual void onCollided(const IntersectionData& intersectionData, std::shared_ptr<Collider> other);
	virtual void update(float delta);
protected:

	virtual void updateMatrix();

	// 게임 오브젝트 세팅
	GameObjectSetting mGameObjectSetting;

	// 트랜스폼
	std::shared_ptr<Transform> mTransform;

	// 모델
	std::shared_ptr<Model> mModel;

	// 콜라이더
	std::shared_ptr<Collider> mCollider;

	// 속도
	std::shared_ptr<DirectX::XMFLOAT3> mVelocity;

	// 가속도
	DirectX::XMFLOAT3 mAcceleration;

	// 질량
	float mMass;
};