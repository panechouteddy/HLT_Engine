#include "pch.h"

int hlt_EntityManager::CreateEntity()
{
	// OPTI POSSIBLE : INVALIDER LES ENTITY DETRUITE AVEC UNE VALEUR DEFINIE
	// EXEMPLE : 0, 1, 2, -1, 4
	int entityID;
	if (m_PoolEntityID.size() == 0)
	{
		entityID = m_countEntityID;
		m_EntityID.push_back(m_countEntityID);
		m_countEntityID++;
	}
	else
	{
		entityID = m_PoolEntityID[0];
		m_EntityID.push_back(entityID);
		std::swap(m_PoolEntityID[0], m_PoolEntityID.back());
		m_PoolEntityID.pop_back();
	}
	std::sort(m_EntityID.begin(), m_EntityID.end());
	return entityID;
}

void hlt_EntityManager::DeleteEntity(int ID)
{
	auto it = std::find(m_EntityID.begin(), m_EntityID.end(), ID);
	if (it == m_EntityID.end())
		return;

	m_pECS->RemoveEntity(*it);
	m_PoolEntityID.push_back(*it);
	m_EntityID.erase(it);
}

void hlt_EntityManager::UpdateMeshTransform()
{
	m_pTransforms.clear();
	m_pMeshs.clear();

	hlt_ECS::ComponentPool<hlt_Component::Transform3D>* transforms = m_pECS->GetComponent<hlt_Component::Transform3D>();
	hlt_ECS::ComponentPool<hlt_Component::Mesh>* meshs = m_pECS->GetComponent<hlt_Component::Mesh>();

	if (transforms == nullptr)
		return;
	for (int i = 0; i < transforms->componentOwnerID.size(); i++)
	{
		Mesh* mesh = &meshs->Get(transforms->componentOwnerID[i])->mesh;
		if (mesh != nullptr)
		{
			m_pTransforms.push_back(&transforms->component[i]->transform);
			m_pMeshs.push_back(mesh);
		}
	}
}
