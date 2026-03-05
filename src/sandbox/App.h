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

	std::vector<Enemy*> GenerateWave(int count);

public:
	int m_Score = 0;

private:
	std::vector<Projectile*> m_vProjs;
	std::vector<Enemy*> m_vEnemys;

	std::vector<int> m_EntityID;
	int m_PlayerID = -1;
	int m_PlayerLife = 10;

	hlt_ECS* ecs;
	hlt_Camera* m_pCamera;

	bool* pIsColliding = nullptr;
	bool* oIsColliding = nullptr;

	int m_Easy = 5;
	int m_Medium = 10;
	int m_Hard = 15;

	hlt_Input::KeyboardInput& keyboardInput = HLT_KEYBOARD;
};

