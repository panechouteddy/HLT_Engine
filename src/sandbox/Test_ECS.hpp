#include "pch.h"

inline int Test_ECS()
{
	if constexpr(DEBUG)
	{
		hlt_DebugConsole::CreateDebugConsole();
	}
	
	hlt_ECS ECS;

	ECS.AddComponent<hlt_Component::ConstantMove>();
	ECS.AddComponent<hlt_Component::Transform3D>();

	hlt_Component::ConstantMove* cMove = ECS.AddComponent<hlt_Component::ConstantMove>(1);
	cMove->move = 1.f;
	cMove->dir = { 0.5f, 0.5f, 0.5f };
	
	hlt_Component::Transform3D* trans = ECS.AddComponent<hlt_Component::Transform3D>(1);

	ECS.AddSystem<hlt_System::ConstantMove>();

	for (int i = 0; i < 1; i++)
	{
		hlt_Time::GetInstance().Update();

		cMove->move = 1.f + i;
		ECS.Update();
		std::cout << trans->transform.pos.x << "|" << trans->transform.pos.y << "|" << trans->transform.pos.z << " : " << hlt_Time::GetInstance().GetDeltaTime() << std::endl;
		Sleep(1000);
	}

	ECS.RemoveComponent<hlt_Component::ConstantMove>(1);
	ECS.RemoveComponent<hlt_Component::ConstantMove>();
	ECS.RemoveComponent<hlt_Component::Transform3D>();
	ECS.RemoveSystem<hlt_System::ConstantMove>();

	if constexpr(DEBUG)
	{
		hlt_DebugConsole::DestroyDebugConsole();
	}

	return 0;
}