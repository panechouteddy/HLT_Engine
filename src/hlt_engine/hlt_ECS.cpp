#include "pch.h"

void hlt_ECS::Update()
{
	for (hlt_System::hlt_SystemClass* system : m_pSystems)
		system->Update();
}

hlt_ECS::~hlt_ECS()
{
	m_Components.clear();
	m_Components.rehash(0);

	for (int i = 0; i < m_pSystems.size(); i++)
		delete m_pSystems[i];
	m_pSystems.clear();
}

void hlt_ECS::Destroy()
{
	for (auto& comp : m_Components)
	{
		delete comp.second;
	}

	for (auto& system : m_pSystems)
	{
		delete system;
	}
}

void hlt_ECS::RemoveEntity(int ID)
{
	for (auto& compPool : m_Components)
	{
		compPool.second->Remove(ID);
	}
}