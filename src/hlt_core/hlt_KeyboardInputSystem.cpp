#include "pch.h"
#include "hlt_Input.h"

void hlt_System::hlt_KeyboardInputSystem::Update(float dt)
{
	hlt_Input::KeyboardInput& keyboard = hlt_Input::KeyboardInput::GetInstance();

	hlt_ECS::ComponentPool<hlt_KeyboardInputComponent>* pKeyboardCPool = m_pECS->GetComponent<hlt_KeyboardInputComponent>();

	for (auto& comp : pKeyboardCPool->component)
	{
		SyncKeySize(comp);
		for (int i = 0; i < comp.keys.size(); i++)
		{
			comp.isKey[i] = keyboard.IsKey(comp.keys[i]);
			comp.isDown[i] = keyboard.IsKeyDown(comp.keys[i]);
			comp.isUp[i] = keyboard.IsKeyUp(comp.keys[i]);
		}
	}
}

void hlt_System::hlt_KeyboardInputSystem::SyncKeySize(hlt_KeyboardInputComponent& comp)
{
	int maxSize = (std::max)({
		comp.isKey.size(),
		comp.isDown.size(),
		comp.isUp.size()
		});
	int keysSize = comp.keys.size();

	if (maxSize > keysSize)
	{
		comp.isKey.resize(keysSize);
		comp.isDown.resize(keysSize);
		comp.isUp.resize(keysSize);
	}
}
