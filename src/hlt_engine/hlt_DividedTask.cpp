#include "pch.h"
#include "hlt_DividedTask.h"

hlt_DividedTask::hlt_DividedTask(int threadsCount)
{
	for (int i = 0; i < threadsCount; i++)
		m_Threads.push_back(new hlt_Thread());
}

hlt_DividedTask::hlt_DividedTask(int threadsCount, METHOD method)
{
	for (int i = 0; i < threadsCount; i++)
		m_Threads.push_back(new hlt_Thread(method));

	m_pMethod = method;
}

hlt_DividedTask::~hlt_DividedTask()
{
	for (int i = 0; i < m_Threads.size(); i++)
		delete m_Threads[i];

	m_Threads.clear();
}

bool hlt_DividedTask::SetMethod(METHOD method)
{
	if (m_IsRunning == true) return false;

	for (int i = 0; i < m_Threads.size(); i++)
		m_Threads[i]->SetMethod(method);

	m_pMethod = method;
}

bool hlt_DividedTask::SetThreadCount(int threadsCount)
{
	if (m_IsRunning == true) return false;

	if (threadsCount < m_Threads.size())
	{
		for (int i = threadsCount - 1; i < m_Threads.size(); i++)
			delete m_Threads[i];
	}
	else if (threadsCount > m_Threads.size())
	{
		for (int i = m_Threads.size() - 1; i < threadsCount; i++)
			m_Threads.push_back(new hlt_Thread(m_pMethod));
	}
	return true;
}

bool hlt_DividedTask::AddThread(int toAdd)
{
	if (m_IsRunning == true) return false;

	for (int i = 0; i < toAdd; i++)
		m_Threads.push_back(new hlt_Thread(m_pMethod));

	return true;
}

void hlt_DividedTask::Run()
{
	if (m_IsRunning == true) return;

	m_IsRunning = true;
	for (int i = 0; i < m_Threads.size(); i++)
		m_Threads[i]->Run();
}

void hlt_DividedTask::Run(METHOD method)
{
	if(SetMethod(method))
		Run();
}

DWORD __stdcall hlt_DividedTask::UpdateIsRunning(void* pParam)
{
	hlt_DividedTask* dividedTask = (hlt_DividedTask*)pParam;

	bool isSomeoneRunning = true;
	while (isSomeoneRunning)
	{
		bool haveToRestart = false;
		for (hlt_Thread* thread : dividedTask->m_Threads)
		{
			if (thread->IsRunning() == true)
			{
				haveToRestart = true;
				break;
			}
		}

		if (haveToRestart == true)
			continue;
		else
			isSomeoneRunning = false;
	}

	return 0;
}
