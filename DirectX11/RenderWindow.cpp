#include "WindowContainer.h"

/// <summary>
/// ���� ���� Ŭ���� �ʱ�ȭ �޼���
/// </summary>
/// <param name="hInstance"></param>
/// <param name="window_title"></param>
/// <param name="window_class"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <returns></returns>
bool RenderWindow::Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
	// Ŭ���� ������Ƽ �ʱ�ȭ
	this->hInstance = hInstance;
	this->width = width;
	this->height = height;
	this->window_title = window_title;
	this->window_title_wide = StringConverter::StringToWide(this->window_title);
	this->window_class = window_class;
	this->window_class_wide = StringConverter::StringToWide(this->window_class);

	// ������ ����ü �ʱ�ȭ
	this->RegisterWindowClass();

	// ������ ����
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

	// ������ ���� ���� üũ
	if (this->handle == NULL)
	{
		ErrorLogger::Log(GetLastError(), "window creation failed for : " + this->window_title);
		return false;
	}

	// ������ â ����
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

	
	if(msg.message == WM_NULL) // ������ â�� �ϳ���� WM_QUIT ���ε� �Ǻ� ����
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
/// ���� ���� Ŭ���� �Ҹ���
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
/// ������ ���ν��� �ݹ�
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
		// lParam ���� �ѱ� WindowContainer ������ �ҷ����� null üũ
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
		if (pWindow == nullptr)
		{
			ErrorLogger::Log("Critical Error : Pointer to window container is null during WM_NCCREATE.");
			exit(-1);
		}

		// ���� ������ ������ �ش� ������ ������ ���� (�Ʒ� �ּ��� �ҷ����� �ڵ�)
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		//WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

		// ������ ���ν��� �Լ� ���� (�����̷��� WindowProc)
		// �� WinProc �ݹ��� �ʱ�ȭ�� ���� �޼����̱� ������
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
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

/// <summary>
/// ������ ����ü �ʱ�ȭ �� ���
/// </summary>
void RenderWindow::RegisterWindowClass()
{
	// ������ ����ü ����
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

	// ������ ����ü ���
	RegisterClassEx(&wc);
}
