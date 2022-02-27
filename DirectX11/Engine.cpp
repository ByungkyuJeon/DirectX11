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
				this->graphics.camera.AdjustRotation((float)me.GetPostY() * 0.01f, (float)me.GetPosX() * 0.01f, 0);
			}
		}
	}

	const float cameraSpeed = 0.51f;

	// key input
	if (mouse.IsRightDown())
	{
		if (keyboard.KeyIsPressed('W'))
		{
			this->graphics.camera.AdjustPosition(DirectX::XMVectorScale(this->graphics.camera.GetForwardVector(), cameraSpeed * frameTime));
		}
		if (keyboard.KeyIsPressed('A'))
		{
			this->graphics.camera.AdjustPosition(DirectX::XMVectorScale(this->graphics.camera.GetLeftVector(), cameraSpeed * frameTime));
		}
		if (keyboard.KeyIsPressed('S'))
		{
			this->graphics.camera.AdjustPosition(DirectX::XMVectorScale(this->graphics.camera.GetBackwardVector(), cameraSpeed * frameTime));
		}
		if (keyboard.KeyIsPressed('D'))
		{
			this->graphics.camera.AdjustPosition(DirectX::XMVectorScale(this->graphics.camera.GetRightVector(), cameraSpeed * frameTime));
		}
		if (keyboard.KeyIsPressed('E'))
		{
			this->graphics.camera.AdjustPosition(DirectX::XMVectorScale(this->graphics.camera.GetUpVector(), cameraSpeed * frameTime));
		}
		if (keyboard.KeyIsPressed('Q'))
		{
			this->graphics.camera.AdjustPosition(DirectX::XMVectorScale(this->graphics.camera.GetDownVector(), cameraSpeed * frameTime));
		}
	}

	// physics engine update
	physicsEngine.Update(frameTime);
}

void Engine::RenderFrame()
{
	graphics.RenderFrame();
}
