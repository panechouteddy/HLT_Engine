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
	m_PlayerID = hlt_Prefab::GameObject::CreateCube();
	m_EntityID.push_back(m_PlayerID);

	ecs = HLT_GAMEMANAGER.GetECS();
	ecs->GetComponent<hlt_Component::Transform3D>(m_PlayerID)->transform.pos = { 0.0f, 0.5f, 0.f };
	hlt_Component::BoxCollider3D* oBox = ecs->AddComponent<hlt_Component::BoxCollider3D>(m_PlayerID);
	oBox->boxType = oBox->OBB;
	oIsColliding = &oBox->isColliding;

	m_pCamera = HLT_CAMERA;

	XMFLOAT3 pos = { 0,0.5f,0.f };
	m_pCamera->m_Transform.pos = pos;
	m_pCamera->m_IsMouseCamera = true;

	/*{
		m_Test2ID = hlt_Prefab::GameObject::CreateCube();
		m_EntityID.push_back(m_Test2ID);

		ecs->GetComponent<hlt_Component::Mesh>(m_Test2ID)->mesh.SetColor(hlt_Color::LightGreen);

		hlt_Component::Transform3D* pTransform = ecs->GetComponent<hlt_Component::Transform3D>(m_Test2ID);
		pTransform->transform.pos = { -5, 0, 15 };
		XMVECTOR angle = XMVectorSet(45.f, 0.f, 0.f, 0.f);
		XMVECTOR rot = XMQuaternionRotationRollPitchYawFromVector(angle);
		pTransform->transform.AddYPR(rot);

		hlt_Component::ConstantMove* pCMove = ecs->AddComponent<hlt_Component::ConstantMove>(m_Test2ID);
		pCMove->dir = { 1.f, 0.f, 0.f };
		pCMove->move = 2.f;

		hlt_Component::BoxCollider3D* pBox = ecs->AddComponent<hlt_Component::BoxCollider3D>(m_Test2ID);
		pBox->boxType = pBox->OBB;
		pIsColliding = &pBox->isColliding;
		XMStoreFloat4(&pBox->boxOBB.m_Box.Orientation, XMQuaternionRotationRollPitchYawFromVector(angle));
	}

	{
		m_TestID = hlt_Prefab::GameObject::CreateCube();
		m_EntityID.push_back(m_TestID);
		ecs->GetComponent<hlt_Component::Mesh>(m_TestID)->mesh.SetColor(hlt_Color::Red);

		hlt_Component::Transform3D* pTransform = ecs->GetComponent<hlt_Component::Transform3D>(m_TestID);
		hlt_Component::Hierarchy* pHierarchy = ecs->AddComponent<hlt_Component::Hierarchy>(m_TestID);
		pHierarchy->parentID = m_Test2ID;
	}

	{
		m_OtherID = hlt_Prefab::GameObject::CreateCube();
		m_EntityID.push_back(m_OtherID);

		ecs->GetComponent<hlt_Component::Transform3D>(m_OtherID)->transform.pos = { 5, 0, 15 };
		hlt_Component::ConstantMove* oCMove = ecs->AddComponent<hlt_Component::ConstantMove>(m_OtherID);
		oCMove->dir = { -1.f, 0.f, 0.f };
		oCMove->move = 4.f;
		hlt_Component::BoxCollider3D* oBox = ecs->AddComponent<hlt_Component::BoxCollider3D>(m_OtherID);
		oBox->boxType = oBox->OBB;
		oIsColliding = &oBox->isColliding;
	}*/

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
			continue;
		}

		m_vEnemys[i]->Update(m_PlayerID, &m_vEnemys);
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

		float spawnOffset = 3.0f;
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
	std::pair<Mesh*, hlt_Transform3D*> object1;
	object1.first = hlt_Prefab::MeshObject::CreateGround();
	hlt_Transform3D* transform1 = new hlt_Transform3D;
	transform1->sca = XMFLOAT3(5.f, 1, 5.f);
	transform1->pos = XMFLOAT3(0, 0, 0);
	transform1->UpdateWorld();
	object1.second = transform1;
	map->MeshContainer.push_back(object1);

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