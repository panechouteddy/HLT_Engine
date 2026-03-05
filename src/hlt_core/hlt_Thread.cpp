#include "hlt_DividedTask.h"
#include "hlt_DividedTask.h"
#include "hlt_DividedTask.h"
#include "hlt_DividedTask.h"
#include "pch.h"
#include "hlt_Thread.h"

hlt_Thread::hlt_Thread()
{
	m_ThreadID = 0;
	m_ParentThreadID = 0;
	m_pMethod = nullptr;
}

hlt_Thread::hlt_Thread(METHOD method)
{
	m_ThreadID = 0;
	m_ParentThreadID = 0;
	m_pMethod = method;
}

hlt_Thread::~hlt_Thread()
{
	if (m_IsRunning == false) return;

	if(m_Thread != nullptr) WaitForSingleObject(m_Thread, INFINITE);

	if (m_Thread != nullptr) CloseHandle(m_Thread);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

bool hlt_Thread::SetMethod(METHOD method)
{
	if (m_IsRunning == true) return false;

	m_pMethod = method;
	return true;
}

void hlt_Thread::Run()
{
	if (m_IsRunning == true || m_pMethod == nullptr) return;

	m_ParentThreadID = GetCurrentThreadId();
	m_ThreadID = GetThreadId(m_Thread);
	m_IsRunning = true;
	CreateThread(NULL, 0, ThreadRunning, this, NULL, NULL);
}

void hlt_Thread::Run(METHOD method)
{
	if(SetMethod(method))
		Run();
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

DWORD WINAPI hlt_Thread::ThreadRunning(void* pParam)
{
	hlt_Thread* pThread = (hlt_Thread*)pParam;

	if (pThread->m_pMethod != nullptr)
		pThread->m_pMethod();

	pThread->m_IsRunning = false;
	return 0;
}
