#include "pch.h"
#include <hlt_core/hlt_CMoveSystem.h>
#include <hlt_core/hlt_CMoveComponent.h>
#include <hlt_core/hlt_Transform3D.h>
#include <hlt_core/DebugTools.h>
#include <hlt_core/hlt_ECS.inl>



int global_ECS()
{
	DebugTool::CreateDebugConsole();

	hlt_ECS ECS;

	ECS.AddComponent<hlt_CMoveComponent>();
	ECS.AddComponent<hlt_Transform3D>();

	hlt_CMoveComponent* cMove = ECS.AddComponent<hlt_CMoveComponent>(1);
	cMove->move = 1.f;
	cMove->dir = { 0.5f, 0.5f, 0.5f };

	ECS.AddComponent<hlt_Transform3D>(1);

	ECS.AddSystem<hlt_CMoveSystem>();

	for (int i = 0; i < 10; i++)
	{
		cMove->move = 1.f + i;
		ECS.Update(1.f);
		Sleep(1000);
	}

	ECS.RemoveComponent<hlt_CMoveComponent>(1);
	ECS.RemoveComponent<hlt_Transform3D>();
	ECS.RemoveSystem<hlt_CMoveSystem>();

	DebugTool::DestroyDebugConsole();

	return 0;
}