#include "pch.h"
#include "Player.h"


Player::Player(hlt_ECS* ecs)
{
	m_pECS = ecs;
	m_ID = hlt_Prefab::GameObject::CreateCube();
	m_pTransform = ecs->GetComponent<hlt_Component::Transform3D>(m_ID);
	m_pTransform->transform.SetScale(0.5f);
	m_pTransform->transform.pos.y = -0.5f;

	hlt_Component::BoxCollider3D* oBox = ecs->AddComponent<hlt_Component::BoxCollider3D>(m_ID);
	oBox->boxType = oBox->AABB;
	m_IsColliding = &oBox->isColliding;
}

void Player::Update()
{
	m_TimeSinceLastHit += HLT_TIME.GetDeltaTime();
}

void Player::TakeDamage()
{
	if (m_TimeSinceLastHit >= m_DamageCooldown)
	{
		m_HP--;
		m_TimeSinceLastHit = 0.0f;
	}
}

bool Player::HaveCollidedWith(int eID)
{
	hlt_Component::BoxCollider3D* ownBox = m_pECS->GetComponent<hlt_Component::BoxCollider3D>(m_ID);

	if (ownBox == nullptr) return false;

	for (int i = 0; i < ownBox->collideWith.size(); i++)
	{
		if (ownBox->collideWith[i] == eID)
			return true;
	}

	return false;
}
