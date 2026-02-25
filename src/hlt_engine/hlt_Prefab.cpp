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
