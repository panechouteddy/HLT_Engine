#include "pch.h"

int global_ECS()
{
	hlt_DebugConsole::CreateDebugConsole();
	
	hlt_ECS ECS;

	ECS.AddComponent<hlt_CMoveComponent>();
	ECS.AddComponent<hlt_Transform3DComponent>();

	hlt_CMoveComponent* cMove = ECS.AddComponent<hlt_CMoveComponent>(1);
	cMove->move = 1.f;
	cMove->dir = { 0.5f, 0.5f, 0.5f };

	ECS.AddComponent<hlt_Transform3DComponent>(1);

	ECS.AddSystem<hlt_CMoveSystem>();

	for (int i = 0; i < 10; i++)
	{
		cMove->move = 1.f + i;
		ECS.Update(1.f);
		Sleep(1000);
	}

	ECS.RemoveComponent<hlt_CMoveComponent>(1);
	ECS.RemoveComponent<hlt_Transform3DComponent>();
	ECS.RemoveSystem<hlt_CMoveSystem>();

	hlt_DebugConsole::DestroyDebugConsole();

	return 0;
}