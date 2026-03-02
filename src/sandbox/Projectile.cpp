#include "pch.h"
#include "Projectile.h"
#include "Enemy.h"

Projectile::Projectile()
{
	m_pos = { 0,0,0 };
	m_dir = { 0,0,0 };
	m_IsDead = false;

	ecs = HLT_GAMEMANAGER.GetECS();

	m_ProjectileID = hlt_Prefab::GameObject::CreatePyramid();

	oCMove = ecs->AddComponent<hlt_Component::ConstantMove>(m_ProjectileID);
	oBox = ecs->AddComponent<hlt_Component::BoxCollider3D>(m_ProjectileID);
}

Projectile::~Projectile()
{
}

void Projectile::Update()
{
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
	oCMove->move = 4.f;

	oBox->boxType = oBox->OBB;
}
