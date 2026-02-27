#pragma once


class App
{
public:
	App();
	~App() = default;

	void OnStart();
	void OnUpdate();
	void OnExit();
	void CreateMap();
	void UpdateTransform(hlt_Transform3D* transform);

private:
	std::vector<int> m_EntityID;
	int m_PlayerID = -1;

	hlt_Camera* m_pCamera;
};

