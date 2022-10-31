#pragma once

#include "Transform.h"
#include "Model.h"
#include "Collider.h"

struct GameObjectSetting
{
public:
	GameObjectSetting() : mIsHiddenInGame{ false }, mCollisionDetectionEnabled{ false }, mIsRigidBody{ false }{}
	GameObjectSetting(bool isHiddenInGame) : 
		mIsHiddenInGame{ isHiddenInGame }, mCollisionDetectionEnabled{ false }, mIsRigidBody{ false }{}

	void setCollisionDetectionState(bool state);
	void setHiddenInGameState(bool state);
	void setRigidBodyState(bool state);

	bool IsHiddenInGame() const;
	bool CollisionDetectionEnabled() const;
	bool IsRigidBody() const;


private:
	bool mIsHiddenInGame;
	bool mCollisionDetectionEnabled;
	bool mIsRigidBody;
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
	void setMass(float mass);
	
	GameObjectSetting& getGameObjectSetting();
	std::shared_ptr<Transform> getTransform() const;
	std::shared_ptr<Model> getModel() const;
	std::shared_ptr<DirectX::XMFLOAT3> getVelocity() const;
	std::shared_ptr<Collider> getCollider() const;
	float getMass() const;

	
	void addForce(const DirectX::XMFLOAT3& force);
	void addForce(float x, float y, float z);
	void multForce(float mult);

	virtual void onCollided(const IntersectionData& intersectionData, std::shared_ptr<Collider> other);
	virtual void update(float delta);
protected:

	virtual void updateMatrix();

	// ���� ������Ʈ ����
	GameObjectSetting mGameObjectSetting;

	// Ʈ������
	std::shared_ptr<Transform> mTransform;

	// ��
	std::shared_ptr<Model> mModel;

	// �ݶ��̴�
	std::shared_ptr<Collider> mCollider;

	// �ӵ�
	std::shared_ptr<DirectX::XMFLOAT3> mVelocity;

	// ����
	float mMass;

	// ź��
	float mElasticity;
};