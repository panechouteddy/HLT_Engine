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
	Start();
	bool isRunning = true;

	while (isRunning)
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
