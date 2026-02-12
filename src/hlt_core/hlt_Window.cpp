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

int hlt_Window::Update()
{
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

bool hlt_Window::CreateWnd(WNDPROC lpfnWndProc)
{
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

void hlt_Window::DestroyWnd()
{
}
