#include "pch.h"
#include "App.h"
#include "Projectile.h"
#include "Enemy.h"
#include "Player.h"

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
	HLT_TIME.SetMaxDeltaTime(30.f);

	ecs = HLT_GAMEMANAGER.GetECS();

	m_pPlayer = new Player(ecs);

	m_pCamera = HLT_CAMERA;

	XMFLOAT3 pos = ecs->GetComponent<hlt_Component::Transform3D>(m_pPlayer->m_ID)->transform.pos;
	m_pCamera->m_Transform.pos = pos;
	m_pCamera->m_IsMouseCamera = true;

	ecs->AddSystem<hlt_System::BoxCollider>();
	ecs->AddSystem<hlt_System::ConstantMove>();
	ecs->AddSystem<hlt_System::hlt_RepulseSystem>();

	m_WarpID = hlt_Prefab::GameObject::CreateCube();
	hlt_Component::BoxCollider3D* warpBox = ecs->AddComponent < hlt_Component::BoxCollider3D>(m_WarpID);
	warpBox->boxType = warpBox->AABB;

	CreateMap();

	m_vEnemys = GenerateWave(m_Difficulty);

	m_LastFrameTime = std::chrono::high_resolution_clock::now();
}

void App::OnUpdate()
{
	m_TimeSinceLastHit += HLT_TIME.GetDeltaTime();

	HLT_D3DAPP->m_TextToDraw = L"Score : " + std::to_wstring(m_pPlayer->GetScore());
	HLT_D3DAPP->m_TextLife = L"PV : " + std::to_wstring(m_pPlayer->GetHP());

	auto currentFrameTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = currentFrameTime - m_LastFrameTime;
	float deltaTime = elapsed.count();
	m_LastFrameTime = currentFrameTime;

	CheckPlayerExit();
	FollowPlayer();

	UpdateDifficulty();
	UpdateEnemies();

	PlayerDied();
	PlayerShoot();

	UpdateShot();

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
	std::uniform_int_distribution<int> dist(0, 5);
	std::random_device rd;
	std::mt19937 gen(rd());
	int level = dist(gen);

	Map_Mesh* map = new Map_Mesh;

	m_MobSpawner.clear();

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

				float positionX = 1 * (x - m_Levels[level].spawnPos.x);
				float positionZ = 1 * (y - m_Levels[level].spawnPos.y);

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

				float positionX = 1 * (x - m_Levels[level].spawnPos.x);
				float groundPositionY = -1;
				float RoofPositionY = 1;
				float positionZ = 1 * (y - m_Levels[level].spawnPos.y);

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

				if (m_Levels[level].grid[x][y] == 'E')
				{
					hlt_Component::Transform3D* warpTransform = ecs->GetComponent<hlt_Component::Transform3D>(m_WarpID);
					if (warpTransform == nullptr && DEBUG) std::cout << "WARP GENERATION ERROR" << std::endl;

					warpTransform->transform.pos = { positionX, 0.f, positionZ };

				}
				if (m_Levels[level].grid[x][y] == 'B')
					m_pPlayer->m_pTransform->transform.pos = { positionX,0,positionZ };
				if (m_Levels[level].grid[x][y] == 'M')
					m_MobSpawner.push_back(XMFLOAT2{ positionX,positionZ });
			}
		}
	}
	HLT_GAMEMANAGER.CreateMap(map);
}

std::vector<Enemy*> App::GenerateWave(int count)
{
	std::vector<Enemy*> enemies(count);
	int i = 0;

	while (count != 0)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> spawner(0,m_MobSpawner.size() -1);

		Enemy* enemy = new Enemy();
		m_EntityID.push_back(enemy->m_EnemyID);

		enemy->m_pos = { m_MobSpawner[spawner(gen)].x, 0.5f, m_MobSpawner[spawner(gen)].y };

		XMVECTOR playerPosVec = XMLoadFloat3(&ecs->GetComponent<hlt_Component::Transform3D>(m_pPlayer->m_ID)->transform.pos);
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
	delete m_pPlayer;
	m_pPlayer = new Player(ecs);

	m_GameEnd = false;
}

void App::UpdateEnemies()
{
	for (int i = 0; i < m_vEnemys.size(); i++)
	{
		if (m_vEnemys[i]->m_IsDead)
		{
			delete m_vEnemys[i];
			m_vEnemys.erase(m_vEnemys.begin() + i);
			i--;
			m_pPlayer->SetScore(m_pPlayer->GetScore() + 1);

			continue;
		}
		m_vEnemys[i]->Update(m_pPlayer->m_ID, &m_vEnemys);

		if(m_vEnemys[i]->m_CollidePlayer)
		{
			m_pPlayer->TakeDamage();
		}
	}

	if (m_vEnemys.empty() && m_GameEnd == false)
	{
		m_vEnemys = GenerateWave(m_Difficulty);
	}
}

void App::UpdateShot()
{
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

void App::UpdateDifficulty()
{
	if (keyboardInput.IsKeyDown(VK_F1))
	{
		m_Difficulty = m_Easy;
		if (m_GameEnd)
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
}

void App::PlayerDied()
{
	if (m_pPlayer->GetHP() <= 0)
	{
		ecs->RemoveEntity(m_pPlayer->m_ID);
		for (int i = 0; i < m_vEnemys.size(); i++)
		{
			m_vEnemys[i]->m_IsDead = true;
			m_vEnemys[i]->Update(m_pPlayer->m_ID, &m_vEnemys);
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
}

void App::PlayerShoot()
{
	if (keyboardInput.IsKeyDown(VK_LBUTTON))
	{
		if(!m_GameEnd)
		{
			Projectile* newBullet = new Projectile();
			m_EntityID.push_back(newBullet->m_ProjectileID);

			XMFLOAT3 playerPos = ecs->GetComponent<hlt_Component::Transform3D>(m_pPlayer->m_ID)->transform.pos;

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

void App::CheckPlayerExit()
{
	if (m_pPlayer->HaveCollidedWith(m_WarpID) == true)
		GenerateMap();
}

void App::FollowPlayer()
{
	if(DEBUG == false)
		m_pCamera->m_Transform.pos = m_pPlayer->m_pTransform->transform.pos;
	//m_pCamera->m_Transform.UpdateWorld();
	hlt_DebugTools::hlt_DebugConsole::PrintVector(m_pCamera->m_Transform.pos);
	hlt_DebugConsole::PrintVector(m_pPlayer->m_pTransform->transform.pos);
}
