#include "pch.h"
#include "Projectile.h"
#include "Enemy.h"

Projectile::Projectile()
{
	m_pos = { 0,0,0 };
	m_dir = { 0,0,0 };

	m_IsDead = false;

	m_Tag = "Ball";

	ecs = HLT_GAMEMANAGER.GetECS();

	m_ProjectileID = hlt_Prefab::GameObject::CreatePyramid();
	ecs->GetComponent<hlt_Component::Transform3D>(m_ProjectileID)->transform.sca = XMFLOAT3(0.5f, 0.5f, 0.5f);
	/*ecs->GetComponent<hlt_Component::Transform3D>(m_ProjectileID)->transform.AddYPR(1.5f, 0, 1.5f);*/

	oCMove = ecs->AddComponent<hlt_Component::ConstantMove>(m_ProjectileID);
	oBox = ecs->AddComponent<hlt_Component::BoxCollider3D>(m_ProjectileID);
}

Projectile::~Projectile()
{
}

void Projectile::Update(int PlayerID)
{
	m_vCollideWith = oBox->collideWith;

	oIsColliding = oBox->isColliding;

	if (oIsColliding == true)
		m_IsDead = true;

	if (m_IsDead)
		ecs->RemoveEntity(m_ProjectileID);
}

void Projectile::Move()
{
	ecs->GetComponent<hlt_Component::Transform3D>(m_ProjectileID)->transform.pos = m_pos;

	oCMove->dir = m_dir;
	oCMove->move = 8.f;

	oBox->boxType = oBox->OBB;
}
