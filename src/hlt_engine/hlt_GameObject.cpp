#include "pch.h"

hlt_ECS* hlt_GameObject::m_pECS = nullptr;

hlt_GameObject::hlt_GameObject(int eID)
{
	m_EntityID = eID;
	m_BornHour = HLT_TIME.GetTotalTime();
}

void hlt_GameObject::Init()
{
	if (m_IsInit == true)
		return;
	if (m_pECS == nullptr)
		HLT_GAMEMANAGER.GetECS();

	m_IsInit = true;
}

float hlt_GameObject::GetAliveSince()
{
	return HLT_TIME.GetTotalTime() - m_BornHour;
}

void hlt_GameObject::Update()
{
	if (m_IsInit == false)
		return;
}

void hlt_GameObject::SetPosition(XMFLOAT3 newPos)
{
	hlt_Component::Transform3D* transform = GetComponent<hlt_Component::Transform3D>();
	if (transform == nullptr)
		return;
	transform->transform.pos = newPos;
}

XMFLOAT3 hlt_GameObject::GetPosition()
{
	hlt_Component::Transform3D* transform = GetComponent<hlt_Component::Transform3D>();
	if (transform == nullptr)
		return XMFLOAT3();
	return transform->transform.pos;
}

void hlt_GameObject::SetScale(XMFLOAT3 newScale)
{
	hlt_Component::Transform3D* transform = GetComponent<hlt_Component::Transform3D>();
	if (transform == nullptr)
		return;
	transform->transform.sca = newScale;
}

XMFLOAT3 hlt_GameObject::GetScale()
{
	hlt_Component::Transform3D* transform = GetComponent<hlt_Component::Transform3D>();
	if (transform == nullptr)
		return XMFLOAT3();
	return transform->transform.sca;
}
