#pragma once
#include <functional>

struct Callable
{
	virtual void Call() = 0;
	virtual ~Callable() = default;
};

template <typename ownerType>
struct CallableWrapper : public Callable
{
	ownerType* m_pObject;
	void (ownerType::* m_pMethod)();

	CallableWrapper(ownerType* obj, void (ownerType::*m)()) : m_pObject(obj), m_pMethod(m) { }

	void Call() override
	{
		if (m_pObject && m_pMethod) (m_pObject->*m_pMethod)();
	}
};

struct hlt_Function
{
	Callable* m_pWrapper = nullptr;
	
	template <typename ownerType>
	void Set(ownerType* obj, void(ownerType::* m)())
	{
		if (m_pWrapper) delete m_pWrapper;
		m_pWrapper = new CallableWrapper<ownerType>(obj, m);
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