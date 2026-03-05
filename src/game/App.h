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
};

