#pragma once


class hlt_GameManager
{
private:
	static hlt_GameManager* s_pInstance;

private:
	hlt_GameManager();
	~hlt_GameManager();

public:
	static hlt_GameManager& GetInstance();
	void Run();

	LRESULT WndProc(HWND& hwnd, UINT& msg, WPARAM& wParam, LPARAM& lParam);

	hlt_ECS* GetECS() { return &m_ECS; }

	int CreateEntity();
	void DeleteEntity(int ID);

private:
	void Start();
	void Update();
	void Render();
	void Destroy();

	void RefreshCore();

private:
	// RUN
	bool m_IsRunning = false;

	// WINDOW
	hlt_Window* m_pWindow = nullptr;

	// ENTITY MANAGER
	int m_countEntityID = 0;
	std::vector<int> m_EntityID;
	std::vector<int> m_PoolEntityID;

	// ECS
	hlt_ECS m_ECS;
};

