#include "pch.h"

void hlt_ECS::Update()
{
	for (hlt_System::hlt_SystemClass* system : m_pSystems)
		system->Update();
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