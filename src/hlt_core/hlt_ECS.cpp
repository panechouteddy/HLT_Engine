#include "pch.h"
#include "hlt_ECS.h"
#include "hlt_System.h"

void hlt_ECS::Update(float dt)
{
	for (hlt_System* system : m_pSystems)
		system->Update(dt);
}
