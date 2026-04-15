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

		hlt_Component::BoxCollider3D* myBox = boxCollider->Get(boxCollider->componentOwnerID[i]);

		if (myBox->boxType == hlt_Component::BoxCollider3D::COUNT)
			continue;
		else if(myBox->boxType == hlt_Component::BoxCollider3D::AABB)
			myBox->boxAABB = myBox->boxAABB + myTransform->transform;
		else if(myBox->boxType == hlt_Component::BoxCollider3D::OBB)
			myBox->boxOBB = myBox->boxOBB + myTransform->transform;

		myBox->isColliding = false;
		myBox->collideWith.clear();
		
		for (int j = 0; j < boxCollider->componentOwnerID.size(); j++)
		{
			if (boxCollider->componentOwnerID[i] == boxCollider->componentOwnerID[j])
				continue;

			hlt_Component::Transform3D* otherTransform = transfrom3D->Get(boxCollider->componentOwnerID[j]);
			if (otherTransform == nullptr)
				continue;

			hlt_Component::BoxCollider3D* otherBox = boxCollider->Get(boxCollider->componentOwnerID[j]);

			if (otherBox->boxType == hlt_Component::BoxCollider3D::COUNT)
				continue;
			else if (otherBox->boxType == hlt_Component::BoxCollider3D::AABB)
			{
				otherBox->boxAABB = otherBox->boxAABB + otherTransform->transform;

				if (myBox->boxType == hlt_Component::BoxCollider3D::AABB && myBox->boxAABB.Contains(otherBox->boxAABB))
				{
					myBox->isColliding = true;
					myBox->collideWith.push_back(boxCollider->componentOwnerID[j]);
				}
				else if (myBox->boxType == hlt_Component::BoxCollider3D::OBB && myBox->boxOBB.Contains(otherBox->boxAABB))
				{
					myBox->isColliding = true;
					myBox->collideWith.push_back(boxCollider->componentOwnerID[j]);
				}
			}
			else if (otherBox->boxType == hlt_Component::BoxCollider3D::OBB)
			{
				otherBox->boxOBB = otherBox->boxOBB + otherTransform->transform;

				if (myBox->boxType == hlt_Component::BoxCollider3D::AABB && myBox->boxAABB.Contains(otherBox->boxOBB))
				{
					myBox->isColliding = true;
					myBox->collideWith.push_back(boxCollider->componentOwnerID[j]);
				}
				else if (myBox->boxType == hlt_Component::BoxCollider3D::OBB && myBox->boxOBB.Contains(otherBox->boxOBB))
				{
					myBox->isColliding = true;
					myBox->collideWith.push_back(boxCollider->componentOwnerID[j]);
				}
			}
		}
	}
}
