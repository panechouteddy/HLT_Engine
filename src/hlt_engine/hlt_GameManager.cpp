#include "pch.h"

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return HLT_GAMEMANAGER.WndProc(hwnd, msg, wParam, lParam);
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

hlt_GameManager* hlt_GameManager::s_pInstance = nullptr;

hlt_GameManager& hlt_GameManager::GetInstance()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new hlt_GameManager();
	}
	return *s_pInstance;
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

hlt_GameManager::hlt_GameManager()
{
}
hlt_GameManager::~hlt_GameManager()
{
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

void hlt_GameManager::Run()
{
	if (m_IsRunning)
		Destroy();

	m_IsRunning = true;
	Start();
	MSG msg = { 0 };

	while (m_IsRunning && msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (GetAsyncKeyState(VK_ESCAPE) > 0)
				m_IsRunning = false;

			if (m_pWindow->IsPaused() == false)
				Update();

			Render();
		}
	}

	Destroy();
}

void hlt_GameManager::Start()
{
	if(DEBUG)
		hlt_DebugTools::hlt_DebugConsole::CreateDebugConsole();

	m_pWindow = &HLT_WINDOW;
	m_pWindow->GetWndName() = L"hlt_Engine Window";
	m_pWindow->GetWndSize() = XMINT2(1080, 720);
	if (m_pWindow->CreateWnd(MainWndProc) == false)
		m_IsRunning = false;

	if(m_pD3D12App == nullptr)
		m_pD3D12App = new D3DApp(m_pWindow);
	if (m_pD3D12App->Initialize() == false)
		m_IsRunning = false;

	if (m_AppFunctions.pStart.pFunction != nullptr && m_AppFunctions.pStart.instance != nullptr)
		m_AppFunctions.pStart.instance;
}

void hlt_GameManager::Update()
{
	m_ECS.Update();

	RefreshCore();

	if (m_AppFunctions.pUpdate != nullptr)
		m_AppFunctions.pUpdate();

	//m_pWindow->Update();
	m_pD3D12App->Update();
}

void hlt_GameManager::Render()
{
	m_pD3D12App->Draw();
	/*m_pD3D12App->StartRender();

	for (Mesh* mesh : ECS.MESH)
		m_pD3D12App->Render(mesh, transform);

	m_pD3D12App->EndRender();*/

	// TO DO
}

void hlt_GameManager::Destroy()
{
	m_ECS.Destroy();
	//m_pWindow->DestroyWnd();

	if (m_pD3D12App != nullptr)
		delete m_pD3D12App;

	if (m_AppFunctions.pExit != nullptr)
		m_AppFunctions.pExit();

	if (DEBUG)
		hlt_DebugTools::hlt_DebugConsole::DestroyDebugConsole();
}

LRESULT hlt_GameManager::WndProc(HWND& hwnd, UINT& msg, WPARAM& wParam, LPARAM& lParam)
{
	switch (msg)
	{
	case WM_MOUSEMOVE:
		HLT_MOUSE.SetMouseMove(lParam);
		break;

	case WM_MOUSEWHEEL:
		HLT_MOUSE.SetMouseWheel(wParam);
		break;

	case WM_ACTIVATE: // PAUSE THE WINDOW
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			m_pWindow->IsPaused() = true;
			//m_Timer.Stop();
		}
		else
		{
			m_pWindow->IsPaused() = false;
			//m_Timer.Start();
		}
		break;

	case WM_SIZING: // TO KEEP A WINDOW RATIO OR A MIN/MAX
		m_pWindow->IsPaused() = true;
		m_pWindow->IsResizing() = true;
		return 0;

	case WM_SIZE: //  TO ADAPT WINDOW SIZE
		m_pWindow->ResizeWnd(lParam);
		if (wParam == SIZE_MINIMIZED)
		{
			m_pWindow->IsPaused() = true;
			m_pWindow->IsMini() = true;
			m_pWindow->IsMaxi() = false;
		}
		else if (wParam == SIZE_MAXIMIZED)
		{
			m_pWindow->IsPaused() = false;
			m_pWindow->IsMini() = false;
			m_pWindow->IsMaxi() = true;
			if(m_pD3D12App != nullptr)
				m_pD3D12App->OnResize();
		}
		else if (wParam == SIZE_RESTORED)
		{
			// Restoring from minimized state?
			if (m_pWindow->IsMini())
			{
				m_pWindow->IsPaused() = false;
				m_pWindow->IsMini() = false;
				if (m_pD3D12App != nullptr)
					m_pD3D12App->OnResize();
			}

			// Restoring from maximized state?
			else if ((m_pWindow->IsMaxi)())
			{
				m_pWindow->IsPaused() = false;
				m_pWindow->IsMaxi() = false;
				if (m_pD3D12App != nullptr)
					m_pD3D12App->OnResize();
			}
			else if (m_pWindow->IsResizing())
			{
				// If user is dragging the resize bars, we do not resize 
				// the buffers here because as the user continuously 
				// drags the resize bars, a stream of WM_SIZE messages are
				// sent to the window, and it would be pointless (and slow)
				// to resize for each WM_SIZE message received from dragging
				// the resize bars.  So instead, we reset after the user is 
				// done resizing the window and releases the resize bars, which 
				// sends a WM_EXITSIZEMOVE message.
			}
			else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
			{
				if (m_pD3D12App != nullptr)
					m_pD3D12App->OnResize();
			}
		}

	case WM_CLOSE:
		if (MessageBox(hwnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
		{
			m_IsRunning = false;
		}
		break;

	case WM_ENTERSIZEMOVE:
		m_pWindow->IsPaused() = true;
		m_pWindow->IsResizing() = true;
		return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		m_pWindow->IsPaused() = false;
		m_pWindow->IsResizing() = false;
		m_pD3D12App->OnResize();
		return 0;

		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	default:
		break;
	}
	//D3DApp::GetApp()->MsgProc(hwnd, msg, wParam, lParam);
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

void hlt_GameManager::RefreshCore()
{
	HLT_KEYBOARD.Update();
	HLT_MOUSE.Update();
	HLT_TIME.Update();
}
