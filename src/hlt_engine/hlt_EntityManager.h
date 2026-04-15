#pragma once


class hlt_EntityManager
{
private:
	int m_countEntityID = 0;
	std::vector<int> m_EntityID;
	std::vector<int> m_PoolEntityID;

	hlt_ECS* m_pECS = nullptr;

	std::vector<std::pair<hlt_Transform3D*, Mesh*>> m_pPosXpMesh;
	std::vector<hlt_Transform3D*> m_pTransforms;
	std::vector<Mesh*> m_pMeshs;

public:
	hlt_EntityManager() = default;
	hlt_EntityManager(hlt_ECS* ecs) { m_pECS = ecs; }
	~hlt_EntityManager() = default;

	void SetECS(hlt_ECS* newECS) { m_pECS = newECS; }

	int CreateEntity();
	void DeleteEntity(int ID);

	void UpdateMeshTransform();
	std::vector<hlt_Transform3D*>& GetTransforms() { return m_pTransforms; }
	std::vector<Mesh*>& GetMeshs() { return m_pMeshs; }
};

