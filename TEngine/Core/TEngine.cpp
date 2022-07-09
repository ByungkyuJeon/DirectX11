#define CREATE_DLL_EXPORTS
#include "TEngine.h"
#include "Framework/Logger/ErrorLogger/TErrorLogger.h"
#include "Physics/PlaneCollider.h"
#include "Physics/SphereCollider.h"

bool TEngine::InternalBootstrap()
{
	if (!m_Window.Initialize())
	{
		PRINT_ERROR("Window Initialization failed.")
		return false;
	}

	if (!m_Renderer.Initialize())
	{
		PRINT_ERROR("Renderer Initialization failed.")
		return false;
	}

	if (!m_Physics.Initialize())
	{
		PRINT_ERROR("Physics Initialization failed.")
			return false;
	}

	if (!m_ModelManager.Initialize())
	{
		PRINT_ERROR("Physics Initialization failed.")
			return false;
	}

	// 임시 테스트
	currentMap = "test";
	this->maps.emplace(currentMap, Map());
	maps[currentMap].addGameObject(std::make_shared<GameObject>(m_Renderer.GetCamera()->getTransform()));

	// 임시 바닥
	{
		std::shared_ptr<Transform> transform = std::make_shared<Transform>();
		transform->setPosition(0.0f, -25.0f, 0.0f);
		std::shared_ptr<DirectX::XMFLOAT3> velocity = std::make_shared<DirectX::XMFLOAT3>(0.0f, 0.0f, 0.0f);
		std::shared_ptr<Model> model = m_ModelManager.Instanciate("Data\\Objects\\plane.fbx");
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(transform, model, velocity);
		gameObject->setMass(1.0f);
		gameObject->getGameObjectSetting().setCollisionDetectionState(true);
		gameObject->getGameObjectSetting().setRigidBodyState(true);
		gameObject->setCollider(std::make_shared<PlaneCollider>(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), -25.0f));
		m_Renderer.RegisterRenderableObject(gameObject);
		m_Physics.registerPhysicsObject(gameObject);
		maps[currentMap].addGameObject(gameObject);
	}


	return true;
}

void TEngine::Update()
{
	static int gen = 0;
	float frameTime = frameTimer.GetMilisecondsElapsed();
	frameTimer.ReStart();

	// camera update state
	static bool cameraUpdated = false;

	KeyboardClass& keyboardClass = m_Window.GetKeyboardClass();
	MouseClass& mouseClass = m_Window.GetMouseClass();
	while (!keyboardClass.CharBufferIsEmpty())
	{
		unsigned char ch = keyboardClass.ReadChar();
	}

	while (!keyboardClass.KeyBufferIsEmpty())
	{
		KeyboardEvent kbe = keyboardClass.ReadKey();
		unsigned char keycode = kbe.GetKeyCode();
	}

	while (!mouseClass.EventBufferIsEmpty())
	{
		MouseEvent me = mouseClass.ReadEvent();
		if (me.GetType() == MouseEvent::EventType::RAW_MOVE)
		{
			if (mouseClass.IsRightDown())
			{
				cameraUpdated = true;
				this->m_Renderer.GetCamera()->getTransform()->rotate((float)me.GetPostY() * 0.01f, (float)me.GetPosX() * 0.01f, 0);
			}
		}
	}

	const float cameraSpeed = 0.51f;

	// key input
	if (mouseClass.IsRightDown())
	{
		cameraUpdated = true;
		if (keyboardClass.KeyIsPressed('W'))
		{
			this->m_Renderer.GetCamera()->getTransform()->translate(DirectX::XMVectorScale(this->m_Renderer.GetCamera()->getTransform()->getForwardVector(), cameraSpeed * frameTime));
		}
		if (keyboardClass.KeyIsPressed('A'))
		{
			this->m_Renderer.GetCamera()->getTransform()->translate(DirectX::XMVectorScale(this->m_Renderer.GetCamera()->getTransform()->getLeftVector(), cameraSpeed * frameTime));
		}
		if (keyboardClass.KeyIsPressed('S'))
		{
			this->m_Renderer.GetCamera()->getTransform()->translate(DirectX::XMVectorScale(this->m_Renderer.GetCamera()->getTransform()->getBackwardVector(), cameraSpeed * frameTime));
		}
		if (keyboardClass.KeyIsPressed('D'))
		{
			this->m_Renderer.GetCamera()->getTransform()->translate(DirectX::XMVectorScale(this->m_Renderer.GetCamera()->getTransform()->getRightVector(), cameraSpeed * frameTime));
		}
		if (keyboardClass.KeyIsPressed('E'))
		{
			this->m_Renderer.GetCamera()->getTransform()->translate(DirectX::XMVectorScale(this->m_Renderer.GetCamera()->getTransform()->getUpVector(), cameraSpeed * frameTime));
		}
		if (keyboardClass.KeyIsPressed('Q'))
		{
			this->m_Renderer.GetCamera()->getTransform()->translate(DirectX::XMVectorScale(this->m_Renderer.GetCamera()->getTransform()->getDownVector(), cameraSpeed * frameTime));
		}
		if (keyboardClass.KeyIsPressed('T'))
		{
			if (++gen > 10)
			{
				gen = 0;
				std::shared_ptr<Transform> transform = std::make_shared<Transform>();
				transform->setPosition(0.0f, 100.0f, 0.0f);
				std::shared_ptr<DirectX::XMFLOAT3> velocity = std::make_shared<DirectX::XMFLOAT3>((std::rand() % 100) * 0.001f, 0.0f, (std::rand() % 100) * 0.001f);
				std::shared_ptr<Model> model = m_ModelManager.Instanciate("Data\\Objects\\sphere.fbx");
				std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(transform, model, velocity);
				gameObject->setMass(1.0f);
				gameObject->getGameObjectSetting().setCollisionDetectionState(true);
				gameObject->setCollider(std::make_shared<SphereCollider>(DirectX::XMFLOAT3(0.0f, 100.0f, 0.0f), 10.0f));
				this->m_Renderer.RegisterRenderableObject(gameObject);
				this->m_Physics.registerPhysicsObject(gameObject);
				maps[currentMap].addGameObject(gameObject);
			}
		}
	}

	if (cameraUpdated)
	{
		cameraUpdated = false;
		this->m_Renderer.GetCamera()->update(frameTime);
	}

	// physics engine update
	m_Physics.update(frameTime);
	m_Renderer.RenderTick();
	this->maps["test"].update(frameTime);
}

const TEngineConfig& TEngine::GetConfig() const
{
	return this->m_EngineConfig;
}

Length TEngine::GetWindowWidth() const
{
	return m_Window.GetWindowData().GetWidth();
}

Length TEngine::GetWindowHeight() const
{
	return m_Window.GetWindowData().GetHeight();
}

bool TEngine::Initialize(HINSTANCE InHInstance)
{
	GWindowInstance = InHInstance;

	try
	{
		InternalBootstrap();
	}
	catch (ErrorCode errCode)
	{
		std::string errorStr = ErrorCodeToString(errCode);
		OutputDebugStringA(errorStr.c_str());
		return false;
	}

	return true;
}

bool TEngine::EngineTick()
{
	if (false == m_Window.ProcessMessages())
	{
		PRINT_ERROR("window process message failed.");
		return false;
	}

	Update();

	return true;
}

DLL_EXPORTS TEngine* ExBootEngine(HINSTANCE InHInstance)
{
	PRINT_STAT("---Engine Bootstrap Started---");

	ExEngine = new TEngine();
	if (!ExEngine->Initialize(InHInstance))
	{
		PRINT_STAT("---Engine Bootstrap Failed---");
		return nullptr;
	}

	PRINT_STAT("---Engine Bootstrap Succeed---");

	return ExEngine;
}
