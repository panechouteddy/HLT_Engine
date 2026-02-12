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

	while (m_IsRunning)
	{
		if (GetAsyncKeyState(VK_ESCAPE) > 0)
			m_IsRunning = false;

		if(m_pWindow->IsPaused() == false)
			Update();

		Render();
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

	RefreshInput();

	m_pWindow->Update();
}

void hlt_GameManager::Render()
{
}

void hlt_GameManager::Destroy()
{
	m_ECS.Destroy();
	m_pWindow->DestroyWnd();

	if (DEBUG)
		hlt_DebugTools::hlt_DebugConsole::DestroyDebugConsole();
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

int hlt_GameManager::CreateEntity()
{
	m_EntityID.push_back(m_countEntityID);
	m_countEntityID++;

	return m_countEntityID - 1;
}

void hlt_GameManager::RefreshInput()
{
	HLT_KEYBOARD.Update();
	HLT_MOUSE.Update();
}

LRESULT hlt_GameManager::WndProc(HWND& hwnd, UINT& msg, WPARAM& wParam, LPARAM& lParam)
{
	switch (msg)
	{
	case WM_MOUSEMOVE:
		HLT_MOUSE.MouseMove(lParam);
		break;

	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
