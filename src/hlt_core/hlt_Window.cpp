#include "pch.h"
#include "hlt_Window.h"

hlt_Window* hlt_Window::s_pInstance = nullptr;

hlt_Window& hlt_Window::GetInstance()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new hlt_Window();
	}
	return *s_pInstance;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

hlt_Window::hlt_Window()
{
}
hlt_Window::~hlt_Window()
{
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

bool hlt_Window::CreateWnd(WNDPROC lpfnWndProc)
{
	m_IsPaused = false;
	m_IsMinimized = false;
	m_IsMaximized = false;
	m_IsResizing = false;
	m_IsFullscreen = false;

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = lpfnWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_WindowInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"MainWnd";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	m_MainWindow = CreateWindowExW(
		0L,
		L"MainWnd",
		m_WindowName.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_WindowSize.x,
		m_WindowSize.y, 
		0, 
		0,
		m_WindowInstance,
		0);

	if (!m_MainWindow)
	{
		printf("Oh shi- %d\n", GetLastError());
		MessageBox(0, L"CreateWindow Failed." + GetLastError(), 0, 0);
		return false;
	}

	ShowWindow(m_MainWindow, SW_SHOW);
	UpdateWindow(m_MainWindow);

	return true;
}

void hlt_Window::SetWndSize(XMINT2 newSize)
{
	m_WindowSize = newSize;

	RECT rc = { 0, 0, m_WindowSize.x, m_WindowSize.y };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	SetWindowPos(m_MainWindow, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);
}

void hlt_Window::ResizeWnd(LPARAM& lParam)
{
	m_WindowSize = XMINT2(LOWORD(lParam), HIWORD(lParam));
}