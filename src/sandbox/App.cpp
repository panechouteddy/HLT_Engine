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
	m_PlayerID = hlt_Prefab::GameObject::CreateCube();
	m_EntityID.push_back(m_PlayerID);
	HLT_GAMEMANAGER.GetECS()->GetComponent<hlt_Component::Transform3D>(m_PlayerID)->transform.pos.z = 5.f;
	HLT_GAMEMANAGER.GetECS()->GetComponent<hlt_Component::Transform3D>(m_PlayerID)->transform.pos.y = 0.5f;
	m_pCamera = HLT_CAMERA;

	XMFLOAT3 pos = { 0,0.5f,5.f };
	m_pCamera->m_Transform.pos = pos;
	m_pCamera->m_IsMouseCamera = true;

	CreateMap();
}

void App::OnUpdate()
{
}

void App::OnExit()
{
}

void App::CreateMap()
{
	Map_Mesh* map = new Map_Mesh;
	std::pair<Mesh*,hlt_Transform3D*> object1;
	object1.first = hlt_Prefab::MeshObject::CreateGround();
	hlt_Transform3D* transform1 = new hlt_Transform3D;
	transform1->UpdateWorld();
	object1.second = transform1;
	map->MeshContainer.push_back(object1);

	/*std::pair<Mesh*, hlt_Transform3D*> object2;
	object2.first = hlt_Prefab::MeshObject::CreateRock();
	hlt_Transform3D* transform2 = new hlt_Transform3D;
	transform2->pos.x = 2;
	transform2->pos.z = 1;
	transform2->UpdateWorld();
	object2.second = transform2;
	map->MeshContainer.push_back(object2);

	std::pair<Mesh*, hlt_Transform3D*> object3;
	object3.first = hlt_Prefab::MeshObject::CreatePyramid();
	hlt_Transform3D* transform3 = new hlt_Transform3D;
	transform3->pos.x = -2;
	transform3->pos.z = 1;
	transform3->UpdateWorld();
	object3.second = transform3;
	map->MeshContainer.push_back(object3);
	*/
	HLT_GAMEMANAGER.CreateMap(map);
}


