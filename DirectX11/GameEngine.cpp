#include "GameEngine.h"

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

	if (!this->mModelFactory.Initialize(this->mGraphics.getDevice(), this->mGraphics.getDeviceContext()))
	{
		return false;
	}

	std::shared_ptr<Transform> temp = std::make_shared<Transform>();
	this->mGraphics.registerRenderableObject(temp, this->mModelFactory.Instanciate("Data\\Objects\\car.fbx"));
	this->maps.emplace("test", Map());
	maps["test"].addGameObject(std::make_shared<GameObject>(temp));
	maps["test"].addGameObject(std::make_shared<GameObject>(this->mGraphics.getCamera()->getTransform()));
	currentMap = "test";

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
	}

	if (cameraUpdated)
	{
		cameraUpdated = false;
		this->mGraphics.getCamera()->update(frameTime);
	}

	// physics engine update
	mPhysicsEngine.Update(frameTime);
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
