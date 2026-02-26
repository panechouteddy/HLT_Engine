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
	hlt_ECS* ecs = HLT_GAMEMANAGER.GetECS();

	m_PlayerID = hlt_Prefab::CreateCube();
	m_EntityID.push_back(m_PlayerID);

	ecs->GetComponent<hlt_Component::Transform3D>(m_PlayerID)->transform.pos = { -5, 0, 15 };
	hlt_Component::ConstantMove* pCMove = ecs->AddComponent<hlt_Component::ConstantMove>(m_PlayerID);
	pCMove->dir = { 0.f, 0.f, 1.f };
	pCMove->move = 10.f;
	hlt_Component::BoxCollider3D* pBox = ecs->AddComponent<hlt_Component::BoxCollider3D>(m_PlayerID);
	pBox->boxType = pBox->AABB;

	m_OtherID = hlt_Prefab::CreateCube();
	m_EntityID.push_back(m_OtherID);

	ecs->GetComponent<hlt_Component::Transform3D>(m_OtherID)->transform.pos = { 5, 0, 15 };
	hlt_Component::ConstantMove* oCMove = ecs->AddComponent<hlt_Component::ConstantMove>(m_OtherID);
	oCMove->dir = { 0.f, 0.f, 1.f };
	oCMove->move = -10.f;
	hlt_Component::BoxCollider3D* oBox = ecs->AddComponent<hlt_Component::BoxCollider3D>(m_OtherID);
	oBox->boxType = oBox->AABB;
	
	ecs->AddComponent<hlt_System::BoxCollider>();
	ecs->AddSystem<hlt_System::ConstantMove>();
}

void App::OnUpdate()
{
}

void App::OnExit()
{
}
