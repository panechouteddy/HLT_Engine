#pragma once
#include "hlt_core/hlt_Transition.h"


class hlt_StateMachine
{
private:
	std::unordered_map<int, hlt_Transition*> m_Transitions;
	int m_NextTransitionCreate = 0;
	int m_CurrentState = -1;

public:
	hlt_StateMachine() = default;
	~hlt_StateMachine() = default;

	void AddTransition(hlt_Transition* newTransition);
	void RemoveTransition(hlt_Transition* toRemove);
	std::unordered_map<int, hlt_Transition*>& GetTransitions() { return m_Transitions; }

	void Update();
};


//template <typename T>
//struct State
//{
//public:
//	T* m_pOwner = nullptr;
//	hlt_ToCall* m_ToCall = nullptr;
//
//public:
//	virtual ~State() = default;
//
//	virtual bool OnEnter() { if (m_ToCall != nullptr) m_ToCall->m_Start.Execute(); }
//	virtual bool OnUpdate(T* owner) { if (m_ToCall != nullptr) m_ToCall->m_Update.Execute(); }
//	virtual bool OnExit(T* owner) { if (m_ToCall != nullptr) m_ToCall->m_Exit.Execute(); }
//};
//
//template <typename T>
//class StateMachine
//{
//private:
//	T* m_pOwner = nullptr;
//	State<T>* m_pState = nullptr;
//
//public:
//	StateMachine(T* owner) { m_pOwner = owner; }
//
//	void ChangeState(State<T>* newState)
//	{
//		if (m_pState != nullptr)
//			m_pState->OnExit(m_pOwner);
//		m_pState = newState;
//		m_pState->OnEnter(m_pOwner);
//	}
//
//	void Update()
//	{
//		if (m_pState != nullptr)
//			m_pState->OnUpdate(m_pOwner);
//	}
//};