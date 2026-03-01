#include "pch.h"
#include "Projectile.h"

Projectile::Projectile()
{
	m_pos = { 0,0,0 };
	m_dir = { 0,0,0 };
	m_IsDead = false;

	ecs = HLT_GAMEMANAGER.GetECS();

	m_ProjectileID = hlt_Prefab::GameObject::CreatePyramid();
}

Projectile::~Projectile()
{
}

void Projectile::Update()
{
	if (m_dir.x != 0 || m_dir.y != 0 || m_dir.z != 0)
		Move();
}

void Projectile::Move()
{
	ecs->GetComponent<hlt_Component::Transform3D>(m_ProjectileID)->transform.pos = m_pos;

	hlt_Component::ConstantMove* oCMove = ecs->AddComponent<hlt_Component::ConstantMove>(m_ProjectileID);
	oCMove->dir = m_dir;
	oCMove->move = 4.f;

	hlt_Component::BoxCollider3D* oBox = ecs->AddComponent<hlt_Component::BoxCollider3D>(m_ProjectileID);
	oBox->boxType = oBox->OBB;
	oIsColliding = &oBox->isColliding;
}
