#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	frameTimer.Start();

	if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height))
	{
		return false;
	}

	if (!graphics.Initialize(render_window.GetHWND(), width, height))
	{
		return false;
	}

	if (!physicsEngine.Initialize())
	{
		return false;
	}

	return true;
}

bool Engine::ProcessMessages()
{
	return this->render_window.ProcessMessages();
}

void Engine::Update()
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
				this->graphics.getCamera()->getTransform()->rotate((float)me.GetPostY() * 0.01f, (float)me.GetPosX() * 0.01f, 0);
			}
		}
	}

	const float cameraSpeed = 0.51f;

	// key input
	if (mouse.IsRightDown())
	{
		if (keyboard.KeyIsPressed('W'))
		{
			this->graphics.getCamera()->getTransform()->translate(DirectX::XMVectorScale(this->graphics.getCamera()->getTransform()->getForwardVector(), cameraSpeed * frameTime));
		}
		if (keyboard.KeyIsPressed('A'))
		{
			this->graphics.getCamera()->getTransform()->translate(DirectX::XMVectorScale(this->graphics.getCamera()->getTransform()->getLeftVector(), cameraSpeed * frameTime));
		}
		if (keyboard.KeyIsPressed('S'))
		{
			this->graphics.getCamera()->getTransform()->translate(DirectX::XMVectorScale(this->graphics.getCamera()->getTransform()->getBackwardVector(), cameraSpeed * frameTime));
		}
		if (keyboard.KeyIsPressed('D'))
		{
			this->graphics.getCamera()->getTransform()->translate(DirectX::XMVectorScale(this->graphics.getCamera()->getTransform()->getRightVector(), cameraSpeed * frameTime));
		}
		if (keyboard.KeyIsPressed('E'))
		{
			this->graphics.getCamera()->getTransform()->translate(DirectX::XMVectorScale(this->graphics.getCamera()->getTransform()->getUpVector(), cameraSpeed * frameTime));
		}
		if (keyboard.KeyIsPressed('Q'))
		{
			this->graphics.getCamera()->getTransform()->translate(DirectX::XMVectorScale(this->graphics.getCamera()->getTransform()->getDownVector(), cameraSpeed * frameTime));
		}
	}

	// physics engine update
	physicsEngine.Update(frameTime);
}

void Engine::RenderFrame()
{
	graphics.RenderFrame();
}

void Engine::LoadMap(const std::string& mapName)
{
}

void Engine::UnloadMap(const std::string& mapName)
{
}
