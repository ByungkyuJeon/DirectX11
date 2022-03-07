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

	this->mGraphics.registerRenderableObject(std::make_shared<Transform>(), this->mModelFactory.Instanciate("Data\\Objects\\car.fbx"));

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
				this->mGraphics.getCamera()->getTransform()->rotate((float)me.GetPostY() * 0.01f, (float)me.GetPosX() * 0.01f, 0);
			}
		}
	}

	const float cameraSpeed = 0.51f;

	// key input
	if (mouse.IsRightDown())
	{
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

	// physics engine update
	mPhysicsEngine.Update(frameTime);
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
