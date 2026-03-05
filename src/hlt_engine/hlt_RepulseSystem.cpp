#include "pch.h"
#include "hlt_RepulseSystem.h"

void hlt_System::hlt_RepulseSystem::Update()
{
	hlt_ECS::ComponentPool<hlt_Component::ConstantMove>* cMoves = m_pECS->GetComponent<hlt_Component::ConstantMove>();
	if (cMoves == nullptr) return;

	hlt_ECS::ComponentPool<hlt_Component::BoxCollider3D>* boxs = m_pECS->GetComponent<hlt_Component::BoxCollider3D>();
	if (boxs == nullptr) return;

	for (int i = 0; i < cMoves->componentOwnerID.size(); i++)
	{
		hlt_Component::BoxCollider3D* ownBox = boxs->Get(cMoves->componentOwnerID[i]);

		if (ownBox == nullptr) continue;

		if(cMoves->component[i]->saveMove != cMoves->component[i]->move)
			cMoves->component[i]->saveMove = cMoves->component[i]->move;

		if (ownBox->isColliding)
			cMoves->component[i]->move = 0.f;
		else
			cMoves->component[i]->move = cMoves->component[i]->saveMove;
	}
}
