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