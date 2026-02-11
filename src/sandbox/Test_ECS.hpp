#include "pch.h"

inline int Test_ECS()
{
	hlt_DebugConsole::CreateDebugConsole();
	
	hlt_ECS ECS;

	ECS.AddComponent<hlt_Component::ConstantMove>();
	ECS.AddComponent<hlt_Component::Transform3D>();

	hlt_Component::ConstantMove* cMove = ECS.AddComponent<hlt_Component::ConstantMove>(1);
	cMove->move = 1.f;
	cMove->dir = { 0.5f, 0.5f, 0.5f };
	
	ECS.AddComponent<hlt_Component::Transform3D>(1);

	ECS.AddSystem<hlt_System::ConstantMove>();

	for (int i = 0; i < 10; i++)
	{
		cMove->move = 1.f + i;
		ECS.Update(1.f);
		Sleep(1000);
	}

	ECS.RemoveComponent<hlt_Component::ConstantMove>(1);
	ECS.RemoveComponent<hlt_Component::Transform3D>();
	ECS.RemoveSystem<hlt_System::ConstantMove>();

	hlt_DebugConsole::DestroyDebugConsole();

	return 0;
}