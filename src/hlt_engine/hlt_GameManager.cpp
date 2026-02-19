#include "pch.h"
#include "hlt_GameManager.h"

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
}

void hlt_GameManager::Update()
{
	m_ECS.Update();

	RefreshCore();

	//m_pWindow->Update();
}

void hlt_GameManager::Render()
{
}

void hlt_GameManager::Destroy()
{
	m_ECS.Destroy();
	//m_pWindow->DestroyWnd();

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

	case WM_SIZING: // TO KEEP A WINDOW RATIO OR A MIN/MAX
		m_pWindow->IsPaused() = true;
		m_pWindow->IsResizing() = true;
		return 0;

	case WM_SIZE: //  TO ADAPT WINDOW SIZE
		m_pWindow->ResizeWnd(lParam);
		m_pWindow->IsPaused() = false;
		m_pWindow->IsResizing() = false;
		return 0;

	case WM_CLOSE:
		if (MessageBox(hwnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
		{
			m_IsRunning = false;
		}
		break;

	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

int hlt_GameManager::CreateEntity()
{
	// OPTI POSSIBLE : INVALIDER LES ENTITY DETRUITE AVEC UNE VALEUR DEFINIE
	// EXEMPLE : 0, 1, 2, -1, 4
	int entityID;
	if(m_PoolEntityID.size() == 0)
	{
		entityID = m_countEntityID;
		m_EntityID.push_back(m_countEntityID);
		m_countEntityID++;
	}
	else
	{
		entityID = m_PoolEntityID[0];
		m_EntityID.push_back(entityID);
		std::swap(m_PoolEntityID[0], m_PoolEntityID.back());
		m_PoolEntityID.pop_back();
	}
	std::sort(m_EntityID.begin(), m_EntityID.end());
	return entityID;
}

void hlt_GameManager::DeleteEntity(int ID)
{
	auto it = std::find(m_EntityID.begin(), m_EntityID.end(), ID);
	if (it == m_EntityID.end())
		return;
	
	m_ECS.RemoveEntity(*it);
	m_PoolEntityID.push_back(*it);
	m_EntityID.erase(it);
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
