#include "pch.h"
#include "hlt_Prefab.h"
#include "DirectXColors.h"

int hlt_Prefab::CreateCube()
{
	hlt_GameManager& gm = hlt_GameManager::GetInstance();

	int cubeID = gm.CreateEntity();
	hlt_ECS* ecs = gm.GetECS();

	ecs->AddComponent<hlt_Component::Transform3D>(cubeID);
	hlt_Component::Mesh* mesh = ecs->AddComponent<hlt_Component::Mesh>(cubeID);

	mesh->mesh.SetMesh("Cube", hlt_Color::Blue);
	gm.AddMesh(&mesh->mesh);

	return cubeID;
}

int hlt_Prefab::CreateCube(XMFLOAT3 pos, XMFLOAT3 sca)
{
	hlt_GameManager& gm = hlt_GameManager::GetInstance();

	int cubeID = gm.CreateEntity();
	hlt_ECS* ecs = gm.GetECS();

	ecs->AddComponent<hlt_Component::Transform3D>(cubeID);
	hlt_Component::Mesh* mesh = ecs->AddComponent<hlt_Component::Mesh>(cubeID);

	
	mesh->mesh.SetMesh("cube", hlt_Color::Blue);
	gm.AddMesh(&mesh->mesh);

	return cubeID;
}

int hlt_Prefab::CreateRock()
{
	hlt_GameManager& gm = hlt_GameManager::GetInstance();

	int cubeID = gm.CreateEntity();
	hlt_ECS* ecs = gm.GetECS();

	ecs->AddComponent<hlt_Component::Transform3D>(cubeID);
	hlt_Component::Mesh* mesh = ecs->AddComponent<hlt_Component::Mesh>(cubeID);


	mesh->mesh.SetMesh("rock", hlt_Color::Gray);
	gm.AddMesh(&mesh->mesh);

	return cubeID;
}

int hlt_Prefab::CreateGround()
{
	hlt_GameManager& gm = hlt_GameManager::GetInstance();

	int cubeID = gm.CreateEntity();
	hlt_ECS* ecs = gm.GetECS();
	std::vector<Vertex> vertices =
	{
		Vertex({ XMFLOAT3(-3.0f, -1.0f, -3.0f) }),
		Vertex({ XMFLOAT3(-3.0f, +1.0f, -3.0f) }),
		Vertex({ XMFLOAT3(+3.0f, +1.0f, -3.0f) }),
		Vertex({ XMFLOAT3(+3.0f, -1.0f, -3.0f) }),
		Vertex({ XMFLOAT3(-3.0f, -1.0f, +3.0f) }),
		Vertex({ XMFLOAT3(-3.0f, +1.0f, +3.0f) }),
		Vertex({ XMFLOAT3(+3.0f, +1.0f, +3.0f) }),
		Vertex({ XMFLOAT3(+3.0f, -1.0f, +3.0f) })
	};

	std::vector<std::uint16_t> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};
	gm.CreateMesh("ground", vertices, indices);
	ecs->AddComponent<hlt_Component::Transform3D>(cubeID);
	hlt_Component::Mesh* mesh = ecs->AddComponent<hlt_Component::Mesh>(cubeID);


	mesh->mesh.SetMesh("ground", hlt_Color::Green);
	gm.AddMesh(&mesh->mesh);

	return cubeID;
}

int hlt_Prefab::CreatePyramid()
{
	hlt_GameManager& gm = hlt_GameManager::GetInstance();

	int pyramidID = gm.CreateEntity();
	hlt_ECS* ecs = gm.GetECS();

	ecs->AddComponent<hlt_Component::Transform3D>(pyramidID);
	hlt_Component::Mesh* mesh = ecs->AddComponent<hlt_Component::Mesh>(pyramidID);

	mesh->mesh.SetMesh("pyramid", hlt_Color::Blue);
	gm.AddMesh(&mesh->mesh);

	return pyramidID;
}
