#pragma once

class Projectile;
class Enemy;

class App
{
private:
	static App* s_pInstance;
public:
	App* GetInstance();

public:
	App();
	~App() = default;

	void OnStart();
	void OnUpdate();
	void OnExit();
	void CreateMap();

private:
	std::vector<Projectile*> m_vProjs;

	std::vector<int> m_EntityID;
	int m_PlayerID = -1;

	hlt_ECS* ecs;
	hlt_Camera* m_pCamera;
	Enemy* m_pEnemy;

	int m_TestID = -1;
	int m_Test2ID = -1;
	int m_OtherID = -1;

	bool* pIsColliding = nullptr;
	bool* oIsColliding = nullptr;
};

