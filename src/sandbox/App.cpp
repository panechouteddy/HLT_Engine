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

	CreateMap();
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
	for (const auto& entry : std::filesystem::directory_iterator(L"..\\..\\res\\Levels"))
	{
		std::ifstream file(entry.path());
		std::string line;
		

		Level* currentLevel = new Level;
		int x = 0;
		int y = 0;

		std::vector<char> linevalue;

		while (std::getline(file, line))
		{
			if (!linevalue.empty())
				currentLevel->grid.push_back(linevalue);
			linevalue.clear();
			y = 0;
			for (char c : line)
			{
				if (c == '-')
				{
					x++;
					m_levels.push_back(*currentLevel);
					currentLevel = new Level;
				}
				else if (c == ' ')
				{
					
					
					continue;
				}
				else
				{
					linevalue.push_back(c);
					y++;
				}
				if (c == 'B')
				{
					currentLevel->spawnPos = { float(x),float(y) };
					;
				}
			}

		}
	}
	GenerateMap();
}

void App::GenerateMap()
{
	int level = 2;

	Map_Mesh* map = new Map_Mesh;


	for (int x = 0;x < m_levels[level].grid.size(); x++)
	{
		for (int y = 0; y < m_levels[level].grid[x].size();y++)
		{
			if (m_levels[level].grid[x][y] == 'W')
			{
				std::pair<Mesh*, hlt_Transform3D> object;

				object.first = hlt_Prefab::MeshObject::CreateCube();
				object.first->SetColor(hlt_Color::DarkGray);
				object.first->SetTexture("bricks2");

				float positionX = 1 * (x - m_levels[level].spawnPos.x) - 4;
				float positionZ = 1 * (y - m_levels[level].spawnPos.y) - 4;

				hlt_Transform3D transform = {};
				transform.pos = { positionX,0,positionZ };
				object.second = transform;

				map->Meshs.push_back(object);
			}
			else
			{
				std::pair<Mesh*, hlt_Transform3D> ground;

				ground.first = hlt_Prefab::MeshObject::CreateCube();
				ground.first->SetColor(hlt_Color::DarkGray);
				ground.first->SetTexture("grass");

				float positionX = 4 * (x - m_levels[level].spawnPos.x);
				float groundPositionY = -1;
				float RoofPositionY = 1;
				float positionZ = 4 * (y - m_levels[level].spawnPos.y);

				hlt_Transform3D transform = {};
				transform.pos = { positionX,groundPositionY,positionZ };
				ground.second = transform;
				map->Meshs.push_back(ground);

				std::pair<Mesh*, hlt_Transform3D> roof;

				roof.first = hlt_Prefab::MeshObject::CreateCube();
				roof.first->SetColor(hlt_Color::DarkGray);
				roof.first->SetTexture("stone");

				transform.pos = { positionX,RoofPositionY,positionZ };
				roof.second = transform;

				map->Meshs.push_back(roof);
			}
		}
	}

	HLT_GAMEMANAGER.CreateMap(map);

}
