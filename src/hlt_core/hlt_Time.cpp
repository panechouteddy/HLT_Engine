#include "pch.h"
#include "hlt_Time.h"


hlt_Time* hlt_Time::s_pInstance = nullptr;

hlt_Time& hlt_Time::GetInstance()
{
    if (s_pInstance == nullptr)
    {
        s_pInstance = new hlt_Time();
    }
    return *s_pInstance;
}

hlt_Time::hlt_Time()
{
    Reset();
}

void hlt_Time::Reset()
{
    m_SystemTime = timeGetTime();
    m_DeltaTime = 0.f;
    m_TotalTime = 0.f;
}

void hlt_Time::Update()
{
    DWORD currentTime = timeGetTime();

    DWORD dt = m_SystemTime - currentTime;
    if (dt > 30)
        dt = 30;

	m_SystemTime = currentTime;
    m_DeltaTime = dt / 1000.f;
    m_TotalTime += m_DeltaTime;
}
