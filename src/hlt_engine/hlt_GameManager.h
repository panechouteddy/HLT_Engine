#pragma once

class hlt_GameManager
{
private:
	static hlt_GameManager* s_pInstance;

private:
	
	~hlt_GameManager();

public:
	hlt_GameManager();
	static hlt_GameManager& GetInstance();
	void Run();

	LRESULT WndProc(HWND& hwnd, UINT& msg, WPARAM& wParam, LPARAM& lParam);

	hlt_ToCall& GetAppMainMethods() { return m_AppToCall; }
	hlt_ECS* GetECS() { return &m_ECS; }

	int CreateEntity() { return m_EntityManager.CreateEntity(); }
	void DeleteEntity(int ID) { return m_EntityManager.DeleteEntity(ID); }

private:
	void Start();
	void Update();
	void Render();
	void Destroy();

	void RefreshCore();

private:
	// RUN
	bool m_IsRunning = false;

	// APP FUNCTIONS
	hlt_ToCall m_AppToCall;
	
	// WINDOW
	hlt_Window* m_pWindow = nullptr;
	D3DApp* m_pD3D12App = nullptr;

	// ENTITY MANAGER
	hlt_EntityManager m_EntityManager;
	/*int m_countEntityID = 0;
	std::vector<int> m_EntityID;
	std::vector<int> m_PoolEntityID;*/

	// ECS
	hlt_ECS m_ECS;
};

