#include "pch.h"

void hlt_System::BoxCollider::Update()
{
	hlt_ECS::ComponentPool<hlt_Component::BoxCollider3D>* boxCollider = m_pECS->GetComponent<hlt_Component::BoxCollider3D>();
	hlt_ECS::ComponentPool<hlt_Component::Transform3D>* transfrom3D = m_pECS->GetComponent<hlt_Component::Transform3D>();

	for (int i = 0; i < boxCollider->componentOwnerID.size(); i++)
	{
		hlt_Component::Transform3D* myTransform = transfrom3D->Get(boxCollider->componentOwnerID[i]);
		if (myTransform == nullptr)
			continue;

		if (boxCollider->component[i].boxType == hlt_Component::BoxCollider3D::COUNT)
			continue;
		else if(boxCollider->component[i].boxType == hlt_Component::BoxCollider3D::AABB)
			boxCollider->component[i].boxAABB = boxCollider->component[i].boxAABB + myTransform->transform;
		else if(boxCollider->component[i].boxType == hlt_Component::BoxCollider3D::OBB)
			boxCollider->component[i].boxAABB = boxCollider->component[i].boxAABB + myTransform->transform;

		boxCollider->component[i].isColliding = false;
		
		for (int j = 0; j < boxCollider->componentOwnerID.size(); j++)
		{
			if (boxCollider->componentOwnerID[i] == boxCollider->componentOwnerID[j])
				continue;

			hlt_Component::Transform3D* otherTransform = transfrom3D->Get(boxCollider->componentOwnerID[j]);
			if (otherTransform == nullptr)
				continue;

			if (boxCollider->component[j].boxType == hlt_Component::BoxCollider3D::COUNT)
				continue;
			else if (boxCollider->component[j].boxType == hlt_Component::BoxCollider3D::AABB)
			{
				boxCollider->component[j].boxAABB = boxCollider->component[j].boxAABB + otherTransform->transform;

				if (boxCollider->component[i].boxType == hlt_Component::BoxCollider3D::AABB && boxCollider->component[i].boxAABB.Contains(boxCollider->component[j].boxAABB))
					boxCollider->component[i].isColliding = true;
				else if (boxCollider->component[i].boxType == hlt_Component::BoxCollider3D::OBB && boxCollider->component[i].boxOBB.Contains(boxCollider->component[j].boxAABB))
					boxCollider->component[i].isColliding = true;
			}
			else if (boxCollider->component[j].boxType == hlt_Component::BoxCollider3D::OBB)
			{
				boxCollider->component[j].boxAABB = boxCollider->component[j].boxAABB + otherTransform->transform;

				if (boxCollider->component[i].boxType == hlt_Component::BoxCollider3D::AABB && boxCollider->component[i].boxAABB.Contains(boxCollider->component[j].boxOBB))
					boxCollider->component[i].isColliding = true;
				else if (boxCollider->component[i].boxType == hlt_Component::BoxCollider3D::OBB && boxCollider->component[i].boxOBB.Contains(boxCollider->component[j].boxOBB))
					boxCollider->component[i].isColliding = true;
			}
		}
	}
}
