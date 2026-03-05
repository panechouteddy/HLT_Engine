#include "pch.h"
#include "SwitchDirectionObject.h"

SwitchDirectionObject::SwitchDirectionObject(hlt_ECS* ecs)
{
	m_pECS = ecs;
	m_ID = HLT_GAMEMANAGER.CreateEntity();

	m_pTransform = ecs->AddComponent<hlt_Component::Transform3D>(m_ID);
	m_pTransform->transform.sca = { 0.5f, 1.f, 0.5f };

	ecs->AddComponent<hlt_Component::Mesh>(m_ID)->mesh.SetMesh("cube", hlt_Color::Aqua);

	m_pCollider = ecs->AddComponent<hlt_Component::BoxCollider3D>(m_ID);
	m_pCollider->boxType = m_pCollider->AABB;
	m_pCollider->boxAABB.Size() = m_pTransform->transform.sca;
	m_pCollider->boxAABB.m_Box.Center = m_pTransform->transform.pos;
}

void SwitchDirectionObject::Update()
{
	for (int i = 0; i < m_pCollider->collideWith.size(); i++)
	{
		hlt_Component::ConstantMove* otherCMove = m_pECS->GetComponent<hlt_Component::ConstantMove>(m_pCollider->collideWith[i]);
		if (otherCMove == nullptr) continue;

		otherCMove->dir = m_NewDirection;
	}
}