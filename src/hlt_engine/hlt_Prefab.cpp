#include "pch.h"
#include "hlt_Prefab.h"
#include "DirectXColors.h"

int hlt_Prefab::GameObject::CreateCube()
{
	hlt_GameManager& gm = hlt_GameManager::GetInstance();

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
	gm.CreateMesh("cube", vertices, indices);
	int cubeID = gm.CreateEntity();
	hlt_ECS* ecs = gm.GetECS();

	ecs->AddComponent<hlt_Component::Transform3D>(cubeID);
	hlt_Component::Mesh* mesh = ecs->AddComponent<hlt_Component::Mesh>(cubeID);

	mesh->mesh.SetMesh("cube", hlt_Color::Blue);

	return cubeID;
}

int hlt_Prefab::GameObject::CreateCube(XMFLOAT3 pos, XMFLOAT3 sca)
{
	hlt_GameManager& gm = hlt_GameManager::GetInstance();

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
	gm.CreateMesh("cube", vertices, indices);
	int cubeID = gm.CreateEntity();
	hlt_ECS* ecs = gm.GetECS();

	ecs->AddComponent<hlt_Component::Transform3D>(cubeID);
	hlt_Component::Mesh* mesh = ecs->AddComponent<hlt_Component::Mesh>(cubeID);

	mesh->mesh.SetMesh("cube", hlt_Color::Blue);

	return cubeID;
}

int hlt_Prefab::GameObject::CreateRock()
{
	hlt_GameManager& gm = hlt_GameManager::GetInstance();

	int cubeID = gm.CreateEntity();
	hlt_ECS* ecs = gm.GetECS();

	ecs->AddComponent<hlt_Component::Transform3D>(cubeID);
	hlt_Component::Transform3D* transform = ecs->AddComponent<hlt_Component::Transform3D>(cubeID);
	hlt_Component::Mesh* mesh = ecs->AddComponent<hlt_Component::Mesh>(cubeID);

	mesh->mesh.SetMesh("rock", hlt_Color::Gray);
	mesh->mesh.SetTexture("stone");
	return cubeID;
}

int hlt_Prefab::GameObject::CreatePyramid()
{
	hlt_GameManager& gm = hlt_GameManager::GetInstance();

	int pyramidID = gm.CreateEntity();
	hlt_ECS* ecs = gm.GetECS();

	ecs->AddComponent<hlt_Component::Transform3D>(pyramidID);
	hlt_Component::Mesh* mesh = ecs->AddComponent<hlt_Component::Mesh>(pyramidID);

	mesh->mesh.SetMesh("pyramid", hlt_Color::Yellow);

	return pyramidID;
}
Mesh* hlt_Prefab::MeshObject::CreateCube()
{

	hlt_GameManager& gm = hlt_GameManager::GetInstance();
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
	gm.CreateMesh("cube", vertices, indices);
	Mesh* m = new Mesh;
	m->SetMesh("cube", hlt_Color::Blue);
	return m;
}
Mesh* hlt_Prefab::MeshObject::CreatePyramid()
{
	hlt_GameManager& gm = hlt_GameManager::GetInstance();
	Mesh* m = new Mesh;
	m->SetMesh("pyramid", hlt_Color::Yellow);
	return m;
}

Mesh* hlt_Prefab::MeshObject::CreateRock()
{
	hlt_GameManager& gm = hlt_GameManager::GetInstance();
	Mesh* m = new Mesh;
	m->SetMesh("rock", hlt_Color::Gray);
	m->SetTexture("stone");
	return m;
}