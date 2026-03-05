#pragma once
#include <chrono>

class Projectile;
class Enemy;
class Player;

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
	void GenerateMap();

	std::vector<Enemy*> GenerateWave(int count);
	void Reset();

private:
	void UpdateEnemies();
	void UpdateShot();
	void UpdateDifficulty();

	void PlayerDied();
	void PlayerShoot();

public:
	int m_Score = 0;

private:
	std::vector<Projectile*> m_vProjs;
	std::vector<Enemy*> m_vEnemys;
	std::vector<Level> m_Levels;

	std::vector<int> m_EntityID;
	Player* m_pPlayer = nullptr;

	hlt_ECS* ecs = nullptr;
	hlt_Camera* m_pCamera = nullptr;

	bool* pIsColliding = nullptr;
	bool* oIsColliding = nullptr;
	bool m_GameEnd = false;

	int m_Easy = 5;
	int m_Medium = 10;
	int m_Hard = 15;

	int m_Difficulty = m_Easy;

	float m_DamageCooldown = 1.0f;
	float m_TimeSinceLastHit = 0.0f;
	std::chrono::high_resolution_clock::time_point m_LastFrameTime;

	hlt_Input::KeyboardInput& keyboardInput = HLT_KEYBOARD;
};

