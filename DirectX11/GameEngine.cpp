#include "GameEngine.h"
#include "Physics/PlaneCollider.h"
#include "Physics/Spherecollider.h"

bool GameEngine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	frameTimer.Start();

	if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height))
	{
		return false;
	}

	if (!this->mGraphics.Initialize(render_window.GetHWND(), width, height))
	{
		return false;
	}

	if (!this->mPhysicsEngine.Initialize())
	{
		return false;
	}

	if (!this->ModelManager.Initialize(this->mGraphics.getDevice(), this->mGraphics.getDeviceContext()))
	{
		return false;
	}

	// 임시 테스트
	currentMap = "test";
	this->maps.emplace(currentMap, Map());
	maps[currentMap].addGameObject(std::make_shared<GameObject>(this->mGraphics.getCamera()->getTransform()));

	// 임시 바닥
	{
		std::shared_ptr<Transform> transform = std::make_shared<Transform>();
		transform->setPosition(0.0f, -25.0f, 0.0f);
		std::shared_ptr<DirectX::XMFLOAT3> velocity = std::make_shared<DirectX::XMFLOAT3>(0.0f, 0.0f, 0.0f);
		std::shared_ptr<Model> model = this->ModelManager.Instanciate("Data\\Objects\\car.fbx");
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(transform, model, velocity);
		gameObject->setMass(1.0f);
		gameObject->getGameObjectSetting().setCollisionDetectionState(true);
		gameObject->getGameObjectSetting().setRigidBodyState(true);
		gameObject->setCollider(std::make_shared<PlaneCollider>(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), -25.0f));
		this->mGraphics.registerRenderableObject(gameObject);
		this->mPhysicsEngine.registerPhysicsObject(gameObject);
		maps[currentMap].addGameObject(gameObject);
	}



	/*{
		std::shared_ptr<Transform> transform = std::make_shared<Transform>();
		transform->setPosition(0.0f, 25.0f, 0.0f);
		std::shared_ptr<DirectX::XMFLOAT3> velocity = std::make_shared<DirectX::XMFLOAT3>(0.0f, 0.0f, 0.0f);
		std::shared_ptr<Model> model = this->ModelManager.Instanciate("Data\\Objects\\sphere.fbx");
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(transform, model, velocity);
		gameObject->setMass(1.0f);
		gameObject->getGameObjectSetting().setCollisionDetectionState(true);
		gameObject->setCollider(std::make_shared<SphereCollider>(DirectX::XMFLOAT3(0.0f, 25.0f, 0.0f), 1.0f));
		this->mGraphics.registerRenderableObject(gameObject);
		this->mPhysicsEngine.registerPhysicsObject(gameObject);
		maps[currentMap].addGameObject(gameObject);
	}*/

	return true;
}

bool GameEngine::ProcessMessages()
{
	return this->render_window.ProcessMessages();
}

void GameEngine::Update()
{
	// frame timing
	float frameTime = frameTimer.GetMilisecondsElapsed();
	frameTimer.ReStart();
	static int gen = 0;

	// camera update state
	static bool cameraUpdated = false;

	while (!keyboard.CharBufferIsEmpty())
	{
		unsigned char ch = keyboard.ReadChar();
	}

	while (!keyboard.KeyBufferIsEmpty())
	{
		KeyboardEvent kbe = keyboard.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();
	}

	while (!mouse.EventBufferIsEmpty())
	{
		MouseEvent me = mouse.ReadEvent();
		if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
		{
			if (mouse.IsRightDown())
			{
				cameraUpdated = true;
				this->mGraphics.getCamera()->getTransform()->rotate((float)me.GetPostY() * 0.01f, (float)me.GetPosX() * 0.01f, 0);
			}
		}
	}

	const float cameraSpeed = 0.51f;

	// key input
	if (mouse.IsRightDown())
	{
		cameraUpdated = true;
		if (keyboard.KeyIsPressed('W'))
		{
			this->mGraphics.getCamera()->getTransform()->translate(DirectX::XMVectorScale(this->mGraphics.getCamera()->getTransform()->getForwardVector(), cameraSpeed * frameTime));
		}
		if (keyboard.KeyIsPressed('A'))
		{
			this->mGraphics.getCamera()->getTransform()->translate(DirectX::XMVectorScale(this->mGraphics.getCamera()->getTransform()->getLeftVector(), cameraSpeed * frameTime));
		}
		if (keyboard.KeyIsPressed('S'))
		{
			this->mGraphics.getCamera()->getTransform()->translate(DirectX::XMVectorScale(this->mGraphics.getCamera()->getTransform()->getBackwardVector(), cameraSpeed * frameTime));
		}
		if (keyboard.KeyIsPressed('D'))
		{
			this->mGraphics.getCamera()->getTransform()->translate(DirectX::XMVectorScale(this->mGraphics.getCamera()->getTransform()->getRightVector(), cameraSpeed * frameTime));
		}
		if (keyboard.KeyIsPressed('E'))
		{
			this->mGraphics.getCamera()->getTransform()->translate(DirectX::XMVectorScale(this->mGraphics.getCamera()->getTransform()->getUpVector(), cameraSpeed * frameTime));
		}
		if (keyboard.KeyIsPressed('Q'))
		{
			this->mGraphics.getCamera()->getTransform()->translate(DirectX::XMVectorScale(this->mGraphics.getCamera()->getTransform()->getDownVector(), cameraSpeed * frameTime));
		}
		if (keyboard.KeyIsPressed('T'))
		{
			if(++gen > 20)
			{
				gen = 0;
				std::shared_ptr<Transform> transform = std::make_shared<Transform>();
				transform->setPosition(0.0f, 50.0f, 0.0f);
				std::shared_ptr<DirectX::XMFLOAT3> velocity = std::make_shared<DirectX::XMFLOAT3>(0.0f, 0.0f, 0.0f);
				std::shared_ptr<Model> model = this->ModelManager.Instanciate("Data\\Objects\\sphere.fbx");
				std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(transform, model, velocity);
				gameObject->setMass(1.0f);
				gameObject->getGameObjectSetting().setCollisionDetectionState(true);
				gameObject->setCollider(std::make_shared<SphereCollider>(DirectX::XMFLOAT3(0.0f, 50.0f, 0.0f), 10.0f));
				this->mGraphics.registerRenderableObject(gameObject);
				this->mPhysicsEngine.registerPhysicsObject(gameObject);
				maps[currentMap].addGameObject(gameObject);
			}
		}
	}

	if (cameraUpdated)
	{
		cameraUpdated = false;
		this->mGraphics.getCamera()->update(frameTime);
	}

	// physics engine update
	mPhysicsEngine.update(frameTime);
	this->maps["test"].update(frameTime);
}

void GameEngine::RenderFrame()
{
	mGraphics.RenderFrame();
}

void GameEngine::LoadMap(const std::string& mapName)
{
}

void GameEngine::UnloadMap(const std::string& mapName)
{
}
