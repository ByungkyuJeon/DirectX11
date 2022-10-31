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
    // 윈도우 데이터 로딩
#pragma message(TODO "윈도우 데이터 로딩 로직")

	// 윈도우 구조체 초기화
	RegisterWindowClass();

	const std::string& windowTitle = gContext->gEngineConfigManager->gWindowConfig->GetWindowTitle();
	TLength windowWidth = gContext->gEngineConfigManager->gWindowConfig->GetWindowWidth();
	TLength windowHeight = gContext->gEngineConfigManager->gWindowConfig->GetWindowHeight();

	// 생성 위치 스크린 중앙으로 설정
	int centerScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - windowWidth / 2;
	int centerScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - windowHeight / 2;

	// 윈도우 영역 보정
	RECT windowRect;
	windowRect.left = centerScreenX;
	windowRect.top = centerScreenY;
	windowRect.right = windowRect.left + windowWidth;
	windowRect.bottom = windowRect.top + windowHeight;
	AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	// 윈도우 생성
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

	// 윈도우 생성 에러 체크
	if (gWindowHandle == NULL)
	{
		PRINT_ERROR("window creation failed");
		return false;
	}

	// 윈도우 창 띄우기
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


	if (msg.message == WM_NULL) // 윈도우 창이 하나라면 WM_QUIT 으로도 판별 가능
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
		// lParam 으로 넘긴 WindowContainer 포인터 불러오고 null 체크
		const CREATESTRUCTW* const createWindowPtr = reinterpret_cast<CREATESTRUCTW*>(lParam);
		TWindow* windowPtr = reinterpret_cast<TWindow*>(createWindowPtr->lpCreateParams);
		if (windowPtr == nullptr)
		{
			PRINT_ERROR("Critical Error : Pointer to window container is null during WM_NCCREATE.");
			exit(-1);
		}

		// 유저 데이터 영역에 해당 윈도우 포인터 저장 (아래 주석은 불러오는 코드)
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(windowPtr));
		//WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		// 윈도우 프로시저 함수 변경 (리다이렉팅 WindowProc)
		// 현 WinProc 콜백은 초기화를 위한 메서드이기 때문에
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
		return windowPtr->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	// 대소문자를 구분함
	case WM_CHAR:
		//unsigned char letter = static_cast<unsigned char>(wParam);
		break;
		// 대소문자 구분하지 않음
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
	// 윈도우 구조체 설정
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

	// 윈도우 구조체 등록
	RegisterClassEx(&windowClass);
}

void TWindow::UnRegisterWindowClass()
{
	UnregisterClass(gContext->gEngineConfigManager->gWindowConfig->GetWindowTitle().c_str(), gWindowInstance);
}
