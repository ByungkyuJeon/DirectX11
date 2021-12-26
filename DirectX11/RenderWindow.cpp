#include "WindowContainer.h"

/// <summary>
/// 렌더 윈도 클래스 초기화 메서드
/// </summary>
/// <param name="hInstance"></param>
/// <param name="window_title"></param>
/// <param name="window_class"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <returns></returns>
bool RenderWindow::Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	// 클래스 프로퍼티 초기화
	this->hInstance = hInstance;
	this->width = width;
	this->height = height;
	this->window_title = window_title;
	this->window_title_wide = StringConverter::StringToWide(this->window_title);
	this->window_class = window_class;
	this->window_class_wide = StringConverter::StringToWide(this->window_class);

	// 윈도우 구조체 초기화
	this->RegisterWindowClass();

	// 윈도우 생성
	this->handle = CreateWindowEx(0,
		this->window_class_wide.c_str(),
		this->window_title_wide.c_str(),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		0,
		0,
		this->width,
		this->height,
		NULL,
		NULL,
		this->hInstance,
		pWindowContainer);

	// 윈도우 생성 에러 체크
	if (this->handle == NULL)
	{
		ErrorLogger::Log(GetLastError(), "window creation failed for : " + this->window_title);
		return false;
	}

	// 윈도우 창 띄우기
	ShowWindow(this->handle, SW_SHOW);
	SetForegroundWindow(this->handle);
	SetFocus(this->handle);

	return true;
}


bool RenderWindow::ProcessMessages()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (PeekMessage(&msg, this->handle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	
	if(msg.message == WM_NULL) // 윈도우 창이 하나라면 WM_QUIT 으로도 판별 가능
	{
		if (!IsWindow(this->handle))
		{
			this->handle = NULL;
			UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
			return false;
		}
	}

	return true;
}

HWND RenderWindow::GetHWND() const
{
	return this->handle;
}

/// <summary>
/// 렌더 윈도 클래스 소멸자
/// </summary>
RenderWindow::~RenderWindow()
{
	if (this->handle != NULL)
	{
		UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
		DestroyWindow(handle);
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
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	}
}

/// <summary>
/// 윈도우 프로시저 콜백
/// </summary>
/// <param name="hwnd"></param>
/// <param name="uMsg"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		// lParam 으로 넘긴 WindowContainer 포인터 불러오고 null 체크
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
		if (pWindow == nullptr)
		{
			ErrorLogger::Log("Critical Error : Pointer to window container is null during WM_NCCREATE.");
			exit(-1);
		}

		// 유저 데이터 영역에 해당 윈도우 포인터 저장 (아래 주석은 불러오는 코드)
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		//WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		// 윈도우 프로시저 함수 변경 (리다이렉팅 WindowProc)
		// 현 WinProc 콜백은 초기화를 위한 메서드이기 때문에
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
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

/// <summary>
/// 윈도우 구조체 초기화 및 등록
/// </summary>
void RenderWindow::RegisterWindowClass()
{
	// 윈도우 구조체 설정
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = this->hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = this->window_class_wide.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	// 윈도우 구조체 등록
	RegisterClassEx(&wc);
}
