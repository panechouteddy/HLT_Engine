#pragma once


class hlt_Time
{
private:
	static hlt_Time* s_pInstance;

private:
	DWORD m_SystemTime;
	float  m_DeltaTime;
	float m_TotalTime;

private:
	hlt_Time();
	~hlt_Time() = default;

public:
	hlt_Time& GetInstance();

	void Reset();

	void Update();

	const float& GetDeltaTime() { return m_DeltaTime; }
	const float& GetTotalTime() { return m_TotalTime; }
};
