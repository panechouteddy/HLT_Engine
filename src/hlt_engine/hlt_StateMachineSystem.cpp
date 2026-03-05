#include "pch.h"

void hlt_System::StateMachine::Update()
{
	hlt_ECS::ComponentPool<hlt_Component::StateMachine>* machines = m_pECS->GetComponent<hlt_Component::StateMachine>();
	if (machines == nullptr) return;

	for (int i = 0; i < machines->componentOwnerID.size(); i++)
	{
		machines->component[i]->m_StateMachine.Update();
	}
}
