#include "pch.h"
#include "Enemy.h"

Enemy::Enemy()
{
	m_pos = { 0,0,0 };
	m_dir = { 0,0,0 };

	m_IsDead = false;

	m_Tag = "Enemy";

	ecs = HLT_GAMEMANAGER.GetECS();

	m_EnemyID = hlt_Prefab::GameObject::CreateCube();

	ecs->GetComponent<hlt_Component::Mesh>(m_EnemyID)->mesh.SetColor(hlt_Color::Crimson);

	oCMove = ecs->AddComponent<hlt_Component::ConstantMove>(m_EnemyID);
	oBox = ecs->AddComponent<hlt_Component::BoxCollider3D>(m_EnemyID);
}

Enemy::~Enemy()
{
}

void Enemy::Update()
{
	oIsColliding = oBox->isColliding;

	if (oIsColliding == true)
		m_IsDead = true;

	if (m_IsDead)
		ecs->RemoveEntity(m_EnemyID);
}

void Enemy::Move()
{
	ecs->GetComponent<hlt_Component::Transform3D>(m_EnemyID)->transform.pos = m_pos;

	oCMove->dir = m_dir;
	oCMove->move = 2.f;

	oBox->boxType = oBox->OBB;
}