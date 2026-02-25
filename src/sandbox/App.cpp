#include "pch.h"
#include "App.h"

App::App()
{
	SETUP_APP_ONSTART(OnStart);
	SETUP_APP_ONUPDATE(OnUpdate);
	SETUP_APP_ONEXIT(OnExit);
}

void App::OnStart()
{
	m_PlayerID = hlt_Prefab::CreateCube();
	m_EntityID.push_back(m_PlayerID);
	HLT_GAMEMANAGER.GetECS()->GetComponent<hlt_Component::Transform3D>(m_PlayerID)->transform.pos.x = 5.f;
}

void App::OnUpdate()
{
}

void App::OnExit()
{
}
