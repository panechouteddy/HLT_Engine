#include "pch.h"

void hlt_System::hlt_Transform3DSystem::Update()
{
	hlt_ECS::ComponentPool<hlt_Component::Transform3D>* transforms = m_pECS->GetComponent<hlt_Component::Transform3D>();

	for (hlt_Component::Transform3D& transformComponent : transforms->component)
	{
		hlt_Transform3D& trans = transformComponent.transform;

		XMVECTOR vPos = XMLoadFloat3(&trans.pos);
		XMVECTOR vSca = XMLoadFloat3(&trans.sca);
		XMVECTOR vRot = XMLoadFloat4(&trans.quaternion);

		XMMATRIX pos = XMMatrixTranslationFromVector(vPos);
		XMMATRIX sca = XMMatrixScalingFromVector(vSca);
		XMMATRIX rot = XMMatrixRotationQuaternion(vRot);

		XMMATRIX world = sca * rot * pos;

		XMStoreFloat4x4(&trans.world, world);
	}
}
