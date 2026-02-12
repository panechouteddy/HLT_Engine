#pragma once

class hlt_Window
{
private:
	static hlt_Window* s_pInstance;

private:
	hlt_Window();
	~hlt_Window();

public:
	static hlt_Window& GetInstance();

	bool CreateWnd(WNDPROC lpfnWndProc);
	void DestroyWnd();

	int Update();

	std::wstring& GetWndName() { return m_WindowName; }

	HWND& GetWnd() { return m_MainWindow; }
	XMINT2& GetWndSize() { return m_WindowSize; }

	bool IsPaused() { return m_IsPaused; }

private:
	std::wstring m_WindowName = L"Basic Window";
	XMINT2 m_WindowSize = { 800, 600 };

	HINSTANCE m_WindowInstance = nullptr;
	HWND      m_MainWindow = nullptr; // main window handle
	bool      m_IsPaused = false;  // is the application paused?
	bool      m_IsMinimized = false;  // is the application minimized?
	bool      m_IsMaximized = false;  // is the application maximized?
	bool      m_IsResizing = false;   // are the resize bars being dragged?
	bool      m_IsFullscreen = false;// fullscreen enabled
};

