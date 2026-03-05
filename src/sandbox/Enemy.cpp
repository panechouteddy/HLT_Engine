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

void Enemy::Update(int m_PlayerID, std::vector<Enemy*>* enemys)
{
	m_vCollideWith = oBox->collideWith;

	oIsColliding = oBox->isColliding;

	m_CollideOther = false;
	m_CollidePlayer = false;

	if (oIsColliding == true)
	{
		for (int j = 0; j < m_vCollideWith.size(); j++) //Collide with
		{
			bool collideEnemy = false;
			for (int i = 0; i < enemys->size(); i++)
			{
				if (m_vCollideWith[j] == (*enemys)[i]->m_EnemyID) //Other Enemy
				{
					m_CollideOther = true;
					collideEnemy = true;
				}
			}

			if (m_vCollideWith[j] == m_PlayerID) //Player
			{
				m_CollidePlayer = true;
			}
			else if (collideEnemy == false) 
			{
				m_CollideOther = false;
				m_CollidePlayer = false;
			}
		}
		if(m_CollideOther == false && m_CollidePlayer == false)
			m_IsDead = true;
	}

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