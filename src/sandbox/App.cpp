#include "pch.h"
#include "App.h"

App* App::s_pInstance = nullptr;

App* App::GetInstance()
{
	if (s_pInstance == nullptr)
		s_pInstance = this;
	return s_pInstance;
}

App::App()
{
	SETUP_APP_ONSTART(OnStart);
	SETUP_APP_ONUPDATE(OnUpdate);
	SETUP_APP_ONEXIT(OnExit);
}

void App::OnStart()
{
	hlt_ECS* ecs = HLT_GAMEMANAGER.GetECS();

			m_PlayerID = hlt_Prefab::GameObject::CreateCube();
		m_EntityID.push_back(m_PlayerID);
	//CreateMap();
}

void App::OnUpdate()
{
	//if (*pIsColliding == true)
	//	HLT_GAMEMANAGER.GetECS()->GetComponent<hlt_Component::ConstantMove>(m_TestID)->move = 0.f;
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

	std::pair<Mesh*, hlt_Transform3D*> object2;
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
	//map->MeshContainer.push_back(object3);

	//HLT_GAMEMANAGER.AddMesh(object1.second, object1.first);
	//HLT_GAMEMANAGER.AddMesh(object2.second, object2.first);
	//HLT_GAMEMANAGER.AddMesh(object3.second, object3.first);

	/*HLT_GAMEMANAGER.AddTransform(object1.second);
	HLT_GAMEMANAGER.AddTransform(object2.second);
	HLT_GAMEMANAGER.AddTransform(object3.second);*/
		
}
