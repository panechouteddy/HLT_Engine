#pragma once
#include <functional>

struct Callable
{
	virtual void Call() = 0;
	virtual ~Callable() = default;
};

template <typename T>
struct CallableWrapper : public Callable
{
	T* m_pObject;
	void (T::* m_pMethod)();

	CallableWrapper(T* obj, void (T::*m)()) : m_pObject(obj), m_pMethod(m) { }

	void Call() override
	{
		if (m_pObject && m_pMethod) (m_pObject->*m_pMethod)();
	}
};

struct hlt_Function
{
	Callable* m_pWrapper = nullptr;
	
	template <typename T>
	void Set(T* obj, void(T::* m)())
	{
		if (m_pWrapper) delete m_pWrapper;
		m_pWrapper = new CallableWrapper<T>(obj, m);
	}

	void Execute()
	{
		if (m_pWrapper) m_pWrapper->Call();
	}

	~hlt_Function() { delete m_pWrapper; }
};

struct hlt_ToCall
{
	hlt_Function m_Start;
	hlt_Function m_Update;
	hlt_Function m_Exit;
};