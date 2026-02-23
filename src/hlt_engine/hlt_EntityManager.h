#pragma once


class hlt_EntityManager
{
private:
	int m_countEntityID = 0;
	std::vector<int> m_EntityID;
	std::vector<int> m_PoolEntityID;

	hlt_ECS* m_pECS = nullptr;

public:
	hlt_EntityManager() = default;
	hlt_EntityManager(hlt_ECS* ecs) { m_pECS = ecs; }
	~hlt_EntityManager() = default;

	int CreateEntity();
	void DeleteEntity(int ID);
};

