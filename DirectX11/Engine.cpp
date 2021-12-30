#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height))
	{
		return false;
	}

	if (!graphics.Initialize(render_window.GetHWND(), width, height))
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

	const DirectX::XMFLOAT3 cameraSpeed{ 0.02f, 0.02f, 0.02f };

	if (mouse.IsRightDown())
	{
		if (keyboard.KeyIsPressed('W'))
		{
			this->graphics.camera.AdjustPosition(DirectX::XMVectorMultiply(this->graphics.camera.GetForwardVector(), DirectX::XMLoadFloat3(&cameraSpeed)));
		}
		if (keyboard.KeyIsPressed('A'))
		{
			this->graphics.camera.AdjustPosition(DirectX::XMVectorMultiply(this->graphics.camera.GetLeftVector(), DirectX::XMLoadFloat3(&cameraSpeed)));
		}
		if (keyboard.KeyIsPressed('S'))
		{
			this->graphics.camera.AdjustPosition(DirectX::XMVectorMultiply(this->graphics.camera.GetBackwardVector(), DirectX::XMLoadFloat3(&cameraSpeed)));
		}
		if (keyboard.KeyIsPressed('D'))
		{
			this->graphics.camera.AdjustPosition(DirectX::XMVectorMultiply(this->graphics.camera.GetRightVector(), DirectX::XMLoadFloat3(&cameraSpeed)));
		}
		if (keyboard.KeyIsPressed('E'))
		{
			this->graphics.camera.AdjustPosition(DirectX::XMVectorMultiply(this->graphics.camera.GetUpVector(), DirectX::XMLoadFloat3(&cameraSpeed)));
		}
		if (keyboard.KeyIsPressed('Q'))
		{
			this->graphics.camera.AdjustPosition(DirectX::XMVectorMultiply(this->graphics.camera.GetDownVector(), DirectX::XMLoadFloat3(&cameraSpeed)));
		}
	}
}

void Engine::RenderFrame()
{
	graphics.RenderFrame();
}
