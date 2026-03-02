#pragma once
#include <hlt_Thread.h>

class hlt_DividedTask
{
public:
	using METHOD = hlt_Thread::METHOD;

private:
	std::vector<hlt_Thread*> m_Threads;
	METHOD m_pMethod = nullptr;
	bool m_IsRunning = false;
	hlt_Thread* m_pUpdateThread = nullptr;

public:
	hlt_DividedTask(int threadsCount);
	hlt_DividedTask(int threadsCount, METHOD method);
	~hlt_DividedTask();

	bool SetMethod(METHOD method);
	bool SetThreadCount(int threadsCount);
	bool AddThread(int toAdd);

	void Run();
	void Run(METHOD method);

private:
	static DWORD WINAPI UpdateIsRunning(void* pParam);
};