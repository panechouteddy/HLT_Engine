#pragma once


class App
{
public:
	App();
	~App() = default;

	void OnStart();
	void OnUpdate();
	void OnExit();

private:
	std::vector<int> m_EntityID;
	int m_PlayerID;
};

