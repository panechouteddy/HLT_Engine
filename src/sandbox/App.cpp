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
	ecs->AddComponent<hlt_Component::Transform3D>(m_PlayerID);
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

	m_vEnemys = GenerateWave(m_Easy);
}

void App::OnUpdate()
{
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
	}

	if (m_vEnemys.empty())
	{
		m_vEnemys = GenerateWave(m_Easy);
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
	Map_Mesh* map = new Map_Mesh;
	std::pair<Mesh*,hlt_Transform3D*> object1;
	object1.first = hlt_Prefab::MeshObject::CreateCube();
	object1.first->SetTexture("grass");
	object1.first->SetMeshVisibility(true);
	object1.first->SetColor(hlt_Color::Green);

	hlt_Transform3D* transform1 = new hlt_Transform3D;
	transform1->pos.y = -4;
	transform1->sca = { 9.5f, 1.5f,9.5f };
	transform1->UpdateWorld();

	object1.second = transform1;
	map->Meshs.push_back(object1);

	/*std::pair<Mesh*, hlt_Transform3D*> object2;
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
	map->Meshs.push_back(object3);*/

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
		std::uniform_real_distribution<float> distXZ(-50.0f, 50.0f);

		Enemy* enemy = new Enemy();
		m_EntityID.push_back(enemy->m_EnemyID);

		enemy->m_pos = { distXZ(gen), 0.5f, distXZ(gen) };

		XMVECTOR playerPosVec = XMLoadFloat3(&ecs->GetComponent<hlt_Component::Transform3D>(m_PlayerID)->transform.pos);
		XMVECTOR enemyPosVec = XMLoadFloat3(&enemy->m_pos);

		XMVECTOR dirVec = XMVectorSubtract(playerPosVec, enemyPosVec);
		dirVec = XMVector3Normalize(dirVec);

		XMStoreFloat3(&enemy->m_dir, dirVec);

		enemy->Move();

		enemies[i] = enemy;
		i++;
		count--;
	}
	return enemies;
}