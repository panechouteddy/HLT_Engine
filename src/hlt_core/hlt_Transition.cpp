#include "pch.h"
#include "hlt_Transition.h"

void hlt_Transition::Update()
{
	for (int i = 0; i < m_Conditions.size(); i++)
	{
		if (m_Conditions[i].Try() == false)
		{
			m_HaveToChange = false;
			return;
		}
	}
	m_HaveToChange = true;
}