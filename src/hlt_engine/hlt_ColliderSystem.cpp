#include "pch.h"

void hlt_System::BoxCollider::Update()
{
	hlt_ECS::ComponentPool<hlt_Component::BoxCollider>* boxCollider = m_pECS->GetComponent<hlt_Component::BoxCollider>();
	hlt_ECS::ComponentPool<hlt_Component::Transform3D>* transfrom3D = m_pECS->GetComponent<hlt_Component::Transform3D>();

	for (int i = 0; i < boxCollider->componentOwnerID.size(); i++)
	{
		hlt_Component::Transform3D* myTransform = transfrom3D->Get(boxCollider->componentOwnerID[i]);
		if (myTransform == nullptr)
			continue;

		hlt_Box::Box3D_AABB myBox = boxCollider->component[i].box + myTransform->transform;
		boxCollider->component[i].isColliding = false;
		
		for (int j = 0; j < boxCollider->componentOwnerID.size(); j++)
		{
			if (boxCollider->componentOwnerID[i] == boxCollider->componentOwnerID[j])
				continue;

			hlt_Component::Transform3D* otherTransform = transfrom3D->Get(boxCollider->componentOwnerID[j]);
			if (otherTransform == nullptr)
				continue;

			hlt_Box::Box3D_AABB otherBox = boxCollider->component[j].box + otherTransform->transform;

			if (myBox.Contains(otherBox))
				boxCollider->component[i].isColliding = true;
		}
	}
}
