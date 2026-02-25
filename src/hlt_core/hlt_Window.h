#pragma once
#include <Windows.h>
#include <DirectXMath.h>

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
	bool CreateWnd(WNDPROC lpfnWndProc, HICON& windowIcon);

	void Update();

	std::wstring& GetWndName() { return m_WindowName; }
	HWND& GetWnd() { return m_MainWindow; }
	HINSTANCE& GetWndInstance() { return m_WindowInstance; }

	bool& IsPaused() { return m_IsPaused; }
	bool& IsMini() { return m_IsMinimized; }
	bool& IsMaxi() { return m_IsMaximized; }
	bool& IsResizing() { return m_IsResizing; }
	bool& IsFullscreen() { return m_IsFullscreen; }

	void SetCursorVisibility(bool isVisible);
	bool IsCursorLocked() { return m_IsCursorLocked; }
	void SetCursorLock(bool isLocked);

	DirectX::XMINT2 GetWndSize() { return m_WindowSize; }
	void SetWndSize(DirectX::XMINT2 newSize);
	void ResizeWnd(LPARAM& lParam);

private:
	std::wstring m_WindowName = L"Basic Window";
	DirectX::XMINT2 m_WindowSize = { 800, 600 };

	HINSTANCE m_WindowInstance = nullptr;
	HWND      m_MainWindow = nullptr; // main window handle
	bool      m_IsPaused = false;  // is the application paused?
	bool      m_IsMinimized = false;  // is the application minimized?
	bool      m_IsMaximized = false;  // is the application maximized?
	bool      m_IsResizing = false;   // are the resize bars being dragged?
	bool      m_IsFullscreen = false;// fullscreen enabled

	bool m_IsCursorLocked = false;
	bool m_IsCursorVisible = true;
};

