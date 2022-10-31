#include "TEngineContext.h"
#include "TWindow.h"
#include "TWindowConfig.h"

TWindow::TWindow()
{

}

TWindow::~TWindow()
{
	UnRegisterWindowClass();
	DestroyWindow(gWindowHandle);
}

bool TWindow::Initialize()
{
    // ������ ������ �ε�
#pragma message(TODO "������ ������ �ε� ����")

	// ������ ����ü �ʱ�ȭ
	RegisterWindowClass();

	const std::string& windowTitle = gContext->gEngineConfigManager->gWindowConfig->GetWindowTitle();
	TLength windowWidth = gContext->gEngineConfigManager->gWindowConfig->GetWindowWidth();
	TLength windowHeight = gContext->gEngineConfigManager->gWindowConfig->GetWindowHeight();

	// ���� ��ġ ��ũ�� �߾����� ����
	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - windowWidth / 2;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - windowHeight / 2;

	// ������ ���� ����
	RECT windowRect;
	windowRect.left = centerScreenX;
	windowRect.top = centerScreenY;
	windowRect.right = windowRect.left + windowWidth;
	windowRect.bottom = windowRect.top + windowHeight;
	AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	// ������ ����
	gWindowHandle = CreateWindowEx(0,
		windowTitle.c_str(),
		windowTitle.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		gWindowInstance,
		this);

	// ������ ���� ���� üũ
	if (gWindowHandle == NULL)
	{
		PRINT_ERROR("window creation failed");
		return false;
	}

	// ������ â ����
	ShowWindow(gWindowHandle, SW_SHOW);
	SetForegroundWindow(gWindowHandle);
	SetFocus(gWindowHandle);

	return true;
}

bool TWindow::ProcessMessages()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (PeekMessage(&msg, gWindowHandle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}


	if (msg.message == WM_NULL) // ������ â�� �ϳ���� WM_QUIT ���ε� �Ǻ� ����
	{
		if (!IsWindow(gWindowHandle))
		{
			gWindowHandle = NULL;
			UnRegisterWindowClass();
			return false;
		}
	}

    return true;
}

LRESULT TWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		// keyboard
	case WM_KEYDOWN:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		if (m_KeyboardClass.IsKeysAutoRepeat())
		{
			m_KeyboardClass.OnKeyPressed(keycode);
		}
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed)
			{
				m_KeyboardClass.OnKeyPressed(keycode);
			}
		}
		return 0;
	}
	case WM_KEYUP:
	{
		unsigned char keycode = static_cast<unsigned char>(wParam);
		m_KeyboardClass.OnKeyReleased(keycode);
		return 0;
	}
	case WM_CHAR:
	{
		unsigned char ch = static_cast<unsigned char> (wParam);
		if (m_KeyboardClass.IsCharsAutoRepeat())
		{
			m_KeyboardClass.OnChar(ch);
		}
		else
		{
			const bool wasPressed = lParam & 0x40000000;
			if (!wasPressed)
			{
				m_KeyboardClass.OnChar(ch);
			}
		}
		return 0;
	}

	// mouse
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		m_MouseClass.OnMouseMove(x, y);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		m_MouseClass.OnLeftPressed(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		m_MouseClass.OnRightPressed(x, y);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		m_MouseClass.OnMiddlePressed(x, y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		m_MouseClass.OnLeftReleased(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		m_MouseClass.OnRightReleased(x, y);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		m_MouseClass.OnMiddleReleased(x, y);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			m_MouseClass.OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			m_MouseClass.OnWheelDown(x, y);
		}
		break;
	}
	case WM_INPUT:
	{
		UINT dataSize;

		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));

		if (dataSize > 0)
		{
			std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					m_MouseClass.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
			}
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;

	default:
	{
		TWindow* const windowPtr = reinterpret_cast<TWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return windowPtr->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		// lParam ���� �ѱ� WindowContainer ������ �ҷ����� null üũ
		const CREATESTRUCTW* const createWindowPtr = reinterpret_cast<CREATESTRUCTW*>(lParam);
		TWindow* windowPtr = reinterpret_cast<TWindow*>(createWindowPtr->lpCreateParams);
		if (windowPtr == nullptr)
		{
			PRINT_ERROR("Critical Error : Pointer to window container is null during WM_NCCREATE.");
			exit(-1);
		}

		// ���� ������ ������ �ش� ������ ������ ���� (�Ʒ� �ּ��� �ҷ����� �ڵ�)
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(windowPtr));
		//WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		// ������ ���ν��� �Լ� ���� (�����̷��� WindowProc)
		// �� WinProc �ݹ��� �ʱ�ȭ�� ���� �޼����̱� ������
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
		return windowPtr->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	// ��ҹ��ڸ� ������
	case WM_CHAR:
		//unsigned char letter = static_cast<unsigned char>(wParam);
		break;
		// ��ҹ��� �������� ����
	case WM_KEYDOWN:
		//unsigned char keycode = static_cast<unsigned char>(wParam);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void TWindow::RegisterWindowClass()
{
	// ������ ����ü ����
	WNDCLASSEX windowClass;
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = HandleMessageSetup;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = gWindowInstance;
	windowClass.hIcon = NULL;
	windowClass.hIconSm = NULL;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = gContext->gEngineConfigManager->gWindowConfig->GetWindowTitle().c_str();
	windowClass.cbSize = sizeof(WNDCLASSEX);

	// ������ ����ü ���
	RegisterClassEx(&windowClass);
}

void TWindow::UnRegisterWindowClass()
{
	UnregisterClass(gContext->gEngineConfigManager->gWindowConfig->GetWindowTitle().c_str(), gWindowInstance);
}
