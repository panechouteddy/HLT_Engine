#pragma once


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
	std::vector<int> m_EntityID;
	int m_PlayerID = -1;

	hlt_Camera* m_pCamera;
	
	int m_TestID = -1;
	int m_OtherID = -1;

	bool* pIsColliding = nullptr;
	bool* oIsColliding = nullptr;
};

