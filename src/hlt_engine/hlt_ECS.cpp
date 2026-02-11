#include "pch.h"

void hlt_ECS::Update(float dt)
{
	for (hlt_System::hlt_SystemClass* system : m_pSystems)
		system->Update(dt);
}
