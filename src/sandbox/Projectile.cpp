#include "pch.h"
#include "Projectile.h"

Projectile::Projectile()
{
	m_pos = { 0,0,0 };
	m_dir = { 0,0,0 };
	m_IsDead = false;

	ecs = HLT_GAMEMANAGER.GetECS();

	m_ProjectileID = hlt_Prefab::GameObject::CreatePyramid();
	oCMove = ecs->AddComponent<hlt_Component::ConstantMove>(m_ProjectileID);
	ecs->SetComponentActive<hlt_Component::ConstantMove>(m_ProjectileID, m_IsActive);

	oBox = ecs->AddComponent<hlt_Component::BoxCollider3D>(m_ProjectileID);
	ecs->SetComponentActive<hlt_Component::BoxCollider3D>(m_ProjectileID, m_IsActive);
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
	if()
	ecs->GetComponent<hlt_Component::Transform3D>(m_ProjectileID)->transform.pos = m_pos;

	oCMove->dir = m_dir;
	oCMove->move = 4.f;

	
	oBox->boxType = oBox->OBB;
	oIsColliding = &oBox->isColliding;
}
