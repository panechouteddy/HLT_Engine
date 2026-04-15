#pragma once
#include "hlt_Condition.h"

class hlt_Transition
{
private:
	std::vector<hlt_Condition> m_Conditions;
	int m_NextState;
	bool m_HaveToChange = false;

public:
	hlt_Transition(int nextState) { m_NextState = nextState; }
	void SetNextState(int nextState) { m_NextState = nextState; }
	int GetNextState() { return m_NextState; }

	void AddCondition(hlt_Condition condition) { m_Conditions.push_back(condition); }

	void Update();
	bool HaveToChange() { return m_HaveToChange; }
};