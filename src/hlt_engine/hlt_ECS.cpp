#include "pch.h"

void hlt_ECS::Update()
{
	for (hlt_System::hlt_SystemClass* system : m_pSystems)
		system->Update();
}

hlt_ECS::~hlt_ECS()
{
	Destroy();

	m_ActiveComponents.clear();
	m_ActiveComponents.rehash(0);
	m_InactiveComponents.clear();
	m_InactiveComponents.rehash(0);

	for (int i = 0; i < m_pSystems.size(); i++)
		delete m_pSystems[i];
	m_pSystems.clear();
}

void hlt_ECS::Destroy()
{
	for (auto& comp : m_ActiveComponents)
	{
		delete comp.second;
	}
	for (auto& comp : m_InactiveComponents)
	{
		delete comp.second;
	}
	for (auto& comp : m_WastedComponents)
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
	for (auto& compPool : m_ActiveComponents)
	{
		compPool.second->Recycle(ID, this);
	}
	for (auto& compPool : m_InactiveComponents)
	{
		compPool.second->Recycle(ID, this);
	}
}