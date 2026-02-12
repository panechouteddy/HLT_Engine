#include "pch.h"

inline int Test_Inputs()
{
	hlt_DebugConsole::CreateDebugConsole();

	hlt_ECS ECS;

	ECS.AddComponent<hlt_Component::Transform3D>(1);
	hlt_Component::hlt_Input::hlt_Keyboard* comp = ECS.AddComponent<hlt_Component::hlt_Input::hlt_Keyboard>(1);

	ECS.GetComponent<hlt_Component::hlt_Input::hlt_Keyboard>(1);

	comp->keys.push_back(VK_Z);
	comp->keys.push_back(VK_Q);
	comp->keys.push_back(VK_S);
	comp->keys.push_back(VK_D);
	comp->keys.push_back(VK_ESCAPE);

	ECS.AddSystem<hlt_System::hlt_Input::hlt_Keyboard>();

	bool run = true;

	while(run)
	{
		ECS.Update();

		if (comp->isKey[0])
			std::cout << "Z";
		if (comp->isKey[4])
			run = false;
	}

	ECS.RemoveComponent<hlt_Component::hlt_Input::hlt_Keyboard>();
	ECS.RemoveComponent<hlt_Component::Transform3D>();
	ECS.RemoveSystem<hlt_System::hlt_Input::hlt_Keyboard>();

	hlt_DebugConsole::DestroyDebugConsole();

	return 0;
}