#include "pch.h"
#include "GameManager.h"

GameManager* GameManager::s_pInstance = nullptr;

GameManager& GameManager::GetInstance()
{
	if (s_pInstance == nullptr)
	{
		s_pInstance = new GameManager();
	}
	return *s_pInstance;
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

GameManager::GameManager()
{
}
GameManager::~GameManager()
{
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

void GameManager::Run()
{
	if (m_IsRunning)
		Destroy();

	Start();
	m_IsRunning = true;

	while (m_IsRunning)
	{
		Update();

		Render();
	}

	Destroy();
}

void GameManager::Start()
{
}

void GameManager::Update()
{
	m_ECS.Update();

	RefreshInput();
}

void GameManager::Render()
{
}

void GameManager::Destroy()
{
	m_ECS.Destroy();
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////

int GameManager::CreateEntity()
{
	m_EntityID.push_back(m_countEntityID);
	m_countEntityID++;

	return m_countEntityID - 1;
}

void GameManager::RefreshInput()
{
	hlt_Input::KeyboardInput::GetInstance().Update();
	hlt_Input::MouseInput::GetInstance().Update();
}

void GameManager::HandleWinMsg()
{
	// --- NOUVEAU : La pompe à messages ---
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT)
			m_IsRunning = false;

		if(msg.message == WM_MOUSEMOVE)

	}
}
