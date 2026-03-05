#include "pch.h"
#include "App.h"
#include "Projectile.h"
#include "Enemy.h"

#include <random>

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
	/*std::string path = "../../res/test.obj";
	hlt_ModelImporter::ImportOBJ(path);*/

	//m_PlayerID = HLT_GAMEMANAGER.CreateEntity();
	m_PlayerID = hlt_Prefab::GameObject::CreateCube();
	m_EntityID.push_back(m_PlayerID);

	ecs = HLT_GAMEMANAGER.GetECS();
	/*hlt_Component::Mesh* mesh = ecs->AddComponent<hlt_Component::Mesh>(m_PlayerID);
	mesh->mesh.SetMesh("path", hlt_Color::White);*/

	ecs->GetComponent<hlt_Component::Transform3D>(m_PlayerID)->transform.pos = { 0.0f, 0.5f, 0.f };
	hlt_Component::BoxCollider3D* oBox = ecs->AddComponent<hlt_Component::BoxCollider3D>(m_PlayerID);
	oBox->boxType = oBox->OBB;
	oIsColliding = &oBox->isColliding;

	m_pCamera = HLT_CAMERA;

	XMFLOAT3 pos = ecs->GetComponent<hlt_Component::Transform3D>(m_PlayerID)->transform.pos;
	m_pCamera->m_Transform.pos = pos;
	m_pCamera->m_IsMouseCamera = true;

	ecs->AddSystem<hlt_System::BoxCollider>();
	ecs->AddSystem<hlt_System::ConstantMove>();
	ecs->AddSystem<hlt_System::hlt_RepulseSystem>();

	CreateMap();

	m_vEnemys = GenerateWave(m_Difficulty);

	m_LastFrameTime = std::chrono::high_resolution_clock::now();
}

void App::OnUpdate()
{
	auto currentFrameTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = currentFrameTime - m_LastFrameTime;
	float deltaTime = elapsed.count();
	m_LastFrameTime = currentFrameTime;

	m_TimeSinceLastHit += deltaTime;

	if (keyboardInput.IsKeyDown(VK_F1)) 
	{
		m_Difficulty = m_Easy;
		if(m_GameEnd)
		{
			Reset();
		}
	}
	else if (keyboardInput.IsKeyDown(VK_F2)) 
	{
		m_Difficulty = m_Medium;
		if (m_GameEnd)
		{
			Reset();
		}
	}
	else if (keyboardInput.IsKeyDown(VK_F3)) 
	{
		m_Difficulty = m_Hard;
		if (m_GameEnd)
		{
			Reset();
		}
	}

	for (int i = 0; i < m_vEnemys.size(); i++)
	{
		if (m_vEnemys[i]->m_IsDead)
		{
			delete m_vEnemys[i];
			m_vEnemys.erase(m_vEnemys.begin() + i);
			i--;
			m_Score++;

			continue;
		}
		
		m_vEnemys[i]->Update(m_PlayerID, &m_vEnemys);

		if (m_TimeSinceLastHit >= m_DamageCooldown)
		{
			m_PlayerLife--;
			m_TimeSinceLastHit = 0.0f;
		}
	}

	if (m_vEnemys.empty() && m_GameEnd == false)
	{
		m_vEnemys = GenerateWave(m_Difficulty);
	}

	
	if (m_PlayerLife <= 0)
	{
		ecs->RemoveEntity(m_PlayerID);
		for (int i = 0; i < m_vEnemys.size(); i++)
		{
			m_vEnemys[i]->m_IsDead = true;
			m_vEnemys[i]->Update(m_PlayerID, &m_vEnemys);
			delete m_vEnemys[i];
			m_vEnemys.erase(m_vEnemys.begin() + i);
		}
		for (int i = 0; i < m_vProjs.size(); i++)
		{
			m_vProjs[i]->m_IsDead = true;
			m_vProjs[i]->Update();
			delete m_vProjs[i];
			m_vProjs.erase(m_vProjs.begin() + i);
		}
		m_GameEnd = true;
	}

	if (keyboardInput.IsKeyDown(VK_LBUTTON))
	{
		Projectile* newBullet = new Projectile();
		m_EntityID.push_back(newBullet->m_ProjectileID);

		XMFLOAT3 playerPos = ecs->GetComponent<hlt_Component::Transform3D>(m_PlayerID)->transform.pos;

		XMMATRIX view = XMLoadFloat4x4(&m_pCamera->m_View);
		XMMATRIX invView = XMMatrixInverse(nullptr, view);

		XMFLOAT3 forward;
		XMStoreFloat3(&forward, invView.r[2]);

		float spawnOffset = 3.f;
		newBullet->m_pos.x = playerPos.x + (forward.x * spawnOffset);
		newBullet->m_pos.y = playerPos.y + (forward.y * spawnOffset);
		newBullet->m_pos.z = playerPos.z + (forward.z * spawnOffset);

		newBullet->m_dir = forward;

		newBullet->Move();

		m_vProjs.push_back(newBullet);
	}
	for (int i = 0; i < m_vProjs.size(); i++)
	{
		if (m_vProjs[i]->m_IsDead)
		{
			delete m_vProjs[i];
			m_vProjs.erase(m_vProjs.begin() + i);
			i--;
			continue;
		}

		m_vProjs[i]->Update();
	}
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
						m_Levels.push_back(*currentLevel);
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


	for (int x = 0; x < m_Levels[level].grid.size(); x++)
	{
		for (int y = 0; y < m_Levels[level].grid[x].size(); y++)
		{
			if (m_Levels[level].grid[x][y] == 'W')
			{
				std::pair<Mesh*, hlt_Transform3D> object;

				object.first = hlt_Prefab::MeshObject::CreateCube();
				object.first->SetColor(hlt_Color::DarkGray);
				object.first->SetTexture("bricks2");

				float positionX = 1 * (x - m_Levels[level].spawnPos.x) - 4;
				float positionZ = 1 * (y - m_Levels[level].spawnPos.y) - 4;

				hlt_Transform3D transform = {};
				transform.pos = { positionX,0,positionZ };
				object.second = transform;

				map->Meshs.push_back(object);
			}
			else
			{
				std::pair<Mesh*, hlt_Transform3D> ground;

				ground.first = hlt_Prefab::MeshObject::CreateCube();
				ground.first = hlt_Prefab::MeshObject::CreateCube();
				ground.first->SetColor(hlt_Color::DarkGray);
				ground.first->SetTexture("grass");

				float positionX = 1 * (x - m_Levels[level].spawnPos.x) - 4;
				float groundPositionY = -1;
				float RoofPositionY = 1;
				float positionZ = 1 * (y - m_Levels[level].spawnPos.y) - 4;

				hlt_Transform3D transform = {};
				transform.pos = { positionX,groundPositionY,positionZ };
			}
		}
	}
}

std::vector<Enemy*> App::GenerateWave(int count)
{
	std::vector<Enemy*> enemies(count);
	int i = 0;

	while (count != 0)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> distXZ(-50.0f, 50.0f);

		Enemy* enemy = new Enemy();
		m_EntityID.push_back(enemy->m_EnemyID);

		enemy->m_pos = { distXZ(gen), 0.5f, distXZ(gen) };

		XMVECTOR playerPosVec = XMLoadFloat3(&ecs->GetComponent<hlt_Component::Transform3D>(m_PlayerID)->transform.pos);
		XMVECTOR enemyPosVec = XMLoadFloat3(&enemy->m_pos);

		XMVECTOR dirVec = XMVectorSubtract(playerPosVec, enemyPosVec);
		dirVec = XMVector3Normalize(dirVec);

		XMStoreFloat3(&enemy->m_dir, dirVec);


		enemies[i] = enemy;
		i++;
		count--;
	}
	return enemies;
}

void App::Reset()
{
	m_PlayerID = hlt_Prefab::GameObject::CreateCube();
	ecs->GetComponent<hlt_Component::Transform3D>(m_PlayerID)->transform.pos = { 0.0f, 0.5f, 0.f };
	hlt_Component::BoxCollider3D* oBox = ecs->AddComponent<hlt_Component::BoxCollider3D>(m_PlayerID);
	oBox->boxType = oBox->OBB;
	oIsColliding = &oBox->isColliding;

	m_PlayerLife = 10;
	m_Score = 0;

	m_GameEnd = false;
}