#pragma once


class GameManager
{
private:
	static GameManager* s_pInstance;

private:
	GameManager();
	~GameManager();

public:
	static GameManager& GetInstance();
	void Run();

	void Start();
	void Update();
	void Render();
	void Destroy();

	hlt_ECS* GetECS() { return &m_ECS; }
	int CreateEntity();

private:
	int m_countEntityID = 0;
	std::vector<int> m_EntityID;

	hlt_ECS m_ECS;
};

