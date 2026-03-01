#include "pch.h"
#include "hlt_StateMachine.h"

void hlt_StateMachine::AddTransition(hlt_Transition* newTransition)
{
	m_Transitions[m_NextTransitionCreate] = newTransition;
	m_NextTransitionCreate++;
}

void hlt_StateMachine::RemoveTransition(hlt_Transition* toRemove)
{
	for (auto& transition : m_Transitions)
	{
		if (transition.second == toRemove)
		{
			delete transition.second;
			m_Transitions.erase(transition.first);
		}
	}
}

void hlt_StateMachine::Update()
{
	if (m_CurrentState == -1)
		return;

	if (m_Transitions.contains(m_CurrentState) == false)
		return;
	
	m_Transitions[m_CurrentState]->Update();
	if (m_Transitions[m_CurrentState]->HaveToChange())
		m_CurrentState = m_Transitions[m_CurrentState]->GetNextState();
}