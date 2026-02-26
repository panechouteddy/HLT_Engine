#include "pch.h"
#include "App.h"

App::App()
{
	SETUP_APP_ONSTART(OnStart);
	SETUP_APP_ONUPDATE(OnUpdate);
	SETUP_APP_ONEXIT(OnExit);
}

void App::OnStart()
{
	m_PlayerID = hlt_Prefab::CreateCube();
	m_EntityID.push_back(m_PlayerID);
	HLT_GAMEMANAGER.GetECS()->GetComponent<hlt_Component::Transform3D>(m_PlayerID)->transform.pos.z = 5.f;

	CreateMap();
}

void App::OnUpdate()
{
}

void App::OnExit()
{
}

void App::CreateMap()
{
	//Map_Mesh* map = new Map_Mesh;
	std::pair<Mesh*,hlt_Transform3D*> object1;
	object1.first = hlt_Prefab::MeshObject::CreateGround();
	hlt_Transform3D* transform1 = new hlt_Transform3D;
	transform1->front={ 0, 0, 1 };
	transform1->quaternion= { 0.f, 0.f, 0.f, 1.f };
	transform1->rotation = {
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f,
	};
	UpdateTransform(transform1);
	object1.second = transform1;
	//map->MeshContainer.push_back(object1);
	std::pair<Mesh*, hlt_Transform3D*> object2;
	object2.first = hlt_Prefab::MeshObject::CreateCube();
	hlt_Transform3D* transform2 = new hlt_Transform3D;
	transform2->front = { 0, 0, 1 };
	transform2->quaternion = { 0.f, 0.f, 0.f, 1.f };
	transform2->rotation = {
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f,
	};
	transform2->pos.x = 2;
	transform2->pos.z = 1;
	UpdateTransform(transform2);
	object2.second = transform2;
	//map->MeshContainer.push_back(object2);
	std::pair<Mesh*, hlt_Transform3D*> object3;
	//object3.first = hlt_Prefab::MeshObject::CreatePyramid();
	object3.first = hlt_Prefab::MeshObject::CreateCube();
	hlt_Transform3D* transform3 = new hlt_Transform3D;
	transform3->front = { 0, 0, 1 };
	transform3->quaternion = { 0.f, 0.f, 0.f, 1.f };
	transform3->rotation = {
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f,
	};
	transform3->pos.x = -2;
	transform3->pos.z = 1;
	UpdateTransform(transform3);
	object3.second = transform3;
	//map->MeshContainer.push_back(object3);

	HLT_GAMEMANAGER.AddMesh(object1.first);
	HLT_GAMEMANAGER.AddMesh(object2.first);
	HLT_GAMEMANAGER.AddMesh(object3.first);

	HLT_GAMEMANAGER.AddTransform(object1.second);
	HLT_GAMEMANAGER.AddTransform(object2.second);
	HLT_GAMEMANAGER.AddTransform(object3.second);
		
}

void App::UpdateTransform(hlt_Transform3D* transform)
{
	XMVECTOR vPos = XMLoadFloat3(&transform->pos);
	XMVECTOR vSca = XMLoadFloat3(&transform->sca);
	XMVECTOR vRot = XMLoadFloat4(&transform->quaternion);

	XMMATRIX pos = XMMatrixTranslationFromVector(vPos);
	XMMATRIX sca = XMMatrixScalingFromVector(vSca);
	XMMATRIX rot = XMMatrixRotationQuaternion(vRot);

	XMMATRIX newWorld = sca * rot * pos;

	XMStoreFloat4x4(&transform->world, newWorld);
}
