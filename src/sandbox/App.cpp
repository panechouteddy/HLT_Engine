#include "pch.h"
#include "App.h"
#include "Projectile.h"
#include "Enemy.h"

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

	HLT_GAMEMANAGER.GetECS()->GetComponent<hlt_Component::Transform3D>(m_PlayerID)->transform.pos = { 0.0f, 0.5f, 5.f };
	m_pCamera = HLT_CAMERA;

	XMFLOAT3 pos = { 0,0.5f,5.f };
	m_pCamera->m_Transform.pos = pos;
	m_pCamera->m_IsMouseCamera = true;
	ecs = HLT_GAMEMANAGER.GetECS();

	{
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
	}

	ecs->AddSystem<hlt_System::BoxCollider>();
	ecs->AddSystem<hlt_System::ConstantMove>();
	ecs->AddSystem<hlt_System::hlt_RepulseSystem>();
	ecs->AddSystem<hlt_System::Hierarchy>();

	m_pEnemy = new Enemy();

	m_pEnemy->m_pos = { 10,0,0 };

	XMVECTOR playerPosVec = XMLoadFloat3(&ecs->GetComponent<hlt_Component::Transform3D>(m_PlayerID)->transform.pos);
	XMVECTOR enemyPosVec = XMLoadFloat3(&m_pEnemy->m_pos);

	XMVECTOR dirVec = XMVectorSubtract(playerPosVec, enemyPosVec);
	dirVec = XMVector3Normalize(dirVec);

	XMStoreFloat3(&m_pEnemy->m_dir, dirVec);

	m_pEnemy->Move();
	// CreateMap();
}

void App::OnUpdate()
{
	hlt_Input::KeyboardInput& keyboardInput = HLT_KEYBOARD;
	//if (keyboardInput.IsKey(VK_TAB))
	//	ecs->SetComponentActive<hlt_Component::ConstantMove>(m_OtherID, false);

	/*if (*pIsColliding == true)
		HLT_GAMEMANAGER.GetECS()->SetComponentActive<hlt_Component::ConstantMove>(m_proj->m_ProjectileID, false);*/

		//	HLT_GAMEMANAGER.GetECS()->GetComponent<hlt_Component::ConstantMove>(m_TestID)->move = 0.f;

	if (keyboardInput.IsKeyDown(VK_TAB))
	{
		Projectile* newBullet = new Projectile();
		m_EntityID.push_back(newBullet->m_ProjectileID);

		newBullet->m_pos = ecs->GetComponent<hlt_Component::Transform3D>(m_PlayerID)->transform.pos;

		XMMATRIX view = XMLoadFloat4x4(&m_pCamera->m_View);
		XMMATRIX invView = XMMatrixInverse(nullptr, view);

		XMFLOAT3 forward;
		XMStoreFloat3(&forward, invView.r[2]);

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
	m_pEnemy->Update();
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
	transform1->UpdateWorld();
	object1.second = transform1;
	map->MeshContainer.push_back(object1);

	/*std::pair<Mesh*, hlt_Transform3D*> object2;
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
	map->MeshContainer.push_back(object3);
	*/
	HLT_GAMEMANAGER.CreateMap(map);
}
