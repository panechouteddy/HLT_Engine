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
	hlt_Camera* GetCamera() { return m_pCamera; }

	int CreateEntity() { return m_EntityManager.CreateEntity(); }
	void DeleteEntity(int ID) { return m_EntityManager.DeleteEntity(ID); }

	void Start();

	void CreateMesh(std::string name, std::vector<Vertex>& vertexList, std::vector<uint16_t>& indexList);
	void CreateMap(Map_Mesh* map);
	void UpdateFps();
	
private:
	void Update();
	void Render();
	void Destroy();

	void RefreshCore();
	void RefreshTransformsMatrix();

private:
	// RUN
	bool m_IsRunning = false;

	// APP FUNCTIONS
	hlt_ToCall m_AppToCall;
	
	// WINDOW
	hlt_Window* m_pWindow = nullptr;
	D3DApp* m_pD3D12App = nullptr;
	hlt_Camera* m_pCamera = nullptr;

	// DEFAULT WINDOW ICON
	HICON m_DefaultIcon;

	// ENTITY MANAGER
	hlt_EntityManager m_EntityManager;

	// ECS
	hlt_ECS m_ECS;
};

