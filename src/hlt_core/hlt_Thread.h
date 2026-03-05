#pragma once

class hlt_Thread
{
public:
	using METHOD = std::function<void()>;

private:
	HANDLE m_Thread = nullptr;
	int m_ThreadID;
	int m_ParentThreadID;
	bool m_IsRunning = false;
	METHOD m_pMethod;

public:
	hlt_Thread();
	hlt_Thread(METHOD method);
	~hlt_Thread();

	bool SetMethod(METHOD method);
	bool IsRunning() { return m_IsRunning; }

	void Run();
	void Run(METHOD method);

protected:
	static DWORD WINAPI ThreadRunning(void* pParam);
};

