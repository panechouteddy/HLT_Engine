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
	std::string path = "../../res/test.obj";
	hlt_ModelImporter::ImportOBJ(path);

	hlt_ECS* ecs = HLT_GAMEMANAGER.GetECS();

	m_PlayerID = HLT_GAMEMANAGER.CreateEntity();
	m_EntityID.push_back(m_PlayerID);

	ecs->AddComponent<hlt_Component::Transform3D>(m_PlayerID);
	hlt_Component::Mesh* mesh = ecs->AddComponent<hlt_Component::Mesh>(m_PlayerID);

	std::vector<Vertex> vertices =
	{
		Vertex({ -1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f }),
		Vertex({ -1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f }),
		Vertex({  1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f }),
		Vertex({  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f }),
		Vertex({ -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f }),
		Vertex({  1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f }),
		Vertex({  1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f }),
		Vertex({ -1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f }),
		Vertex({ -1.0f,  1.0f, -1.0f }, { 0.0f, 1.0f }),
		Vertex({ -1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f }),
		Vertex({  1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f }),
		Vertex({  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f }),
		Vertex({ -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f }),
		Vertex({  1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f }),
		Vertex({  1.0f, -1.0f,  1.0f }, { 0.0f, 0.0f }),
		Vertex({ -1.0f, -1.0f,  1.0f }, { 1.0f, 0.0f }),
		Vertex({ -1.0f, -1.0f,  1.0f }, { 0.0f, 1.0f }),
		Vertex({ -1.0f,  1.0f,  1.0f }, { 0.0f, 0.0f }),
		Vertex({ -1.0f,  1.0f, -1.0f }, { 1.0f, 0.0f }),
		Vertex({ -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f }),
		Vertex({  1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f }),
		Vertex({  1.0f,  1.0f, -1.0f }, { 0.0f, 0.0f }),
		Vertex({  1.0f,  1.0f,  1.0f }, { 1.0f, 0.0f }),
		Vertex({  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f }),
	};

	std::vector<std::uint16_t> indices =
	{
		0,1,2,  0,2,3,
		4,5,6,  4,6,7,
		8,9,10, 8,10,11,
		12,13,14, 12,14,15,
		16,17,18, 16,18,19,
		20,21,22, 20,22,23
	};

	D3DApp::GetApp()->CreateOriginalMesh("test", vertices, indices);

	mesh->mesh.SetMesh("test", hlt_Color::LightGreen);

	//CreateMap();
}

void App::OnUpdate()
{
	//if (*pIsColliding == true)
	//	HLT_GAMEMANAGER.GetECS()->SetComponentActive<hlt_Component::Mesh>(m_TestID, false);
	//	HLT_GAMEMANAGER.GetECS()->GetComponent<hlt_Component::ConstantMove>(m_TestID)->move = 0.f;
}

void App::OnExit()
{
}

void App::CreateMap()
{
	Map_Mesh* map = new Map_Mesh;
	std::pair<Mesh*,hlt_Transform3D*> object1;
	object1.first = hlt_Prefab::MeshObject::CreateCube();
	object1.first->SetTexture("grass");
	object1.first->SetMeshVisibility(true);
	object1.first->SetColor(hlt_Color::Green);

	hlt_Transform3D* transform1 = new hlt_Transform3D;
	transform1->pos.y = -4;
	transform1->sca = { 5.f, 1.5f,5.f };
	transform1->UpdateWorld();

	object1.second = transform1;
	map->Meshs.push_back(object1);

	std::pair<Mesh*, hlt_Transform3D*> object2;
	object2.first = hlt_Prefab::MeshObject::CreateRock();
	object2.first->SetMeshVisibility(true);

	hlt_Transform3D* transform2 = new hlt_Transform3D;
	transform2->pos.x = 4;
	transform2->pos.z = 1;
	transform2->UpdateWorld();
	object2.second = transform2;
	map->Meshs.push_back(object2);

	std::pair<Mesh*, hlt_Transform3D*> object3;
	object3.first = hlt_Prefab::MeshObject::CreatePyramid();
	object3.first->SetTexture("bricks3");
	object3.first->SetMeshVisibility(true);

	hlt_Transform3D* transform3 = new hlt_Transform3D;
	transform3->pos.x = -2;
	transform3->pos.z = 1;
	transform3->UpdateWorld();
	object3.second = transform3;
	map->Meshs.push_back(object3);

	HLT_GAMEMANAGER.CreateMap(map);
}
