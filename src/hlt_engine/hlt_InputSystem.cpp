#include "pch.h"
#include "hlt_Input.h"

void hlt_System::hlt_Input::hlt_Keyboard::Update()
{
	::hlt_Input::KeyboardInput& keyboard = ::hlt_Input::KeyboardInput::GetInstance();
	keyboard.Update();

	hlt_ECS::ComponentPool<hlt_Component::hlt_Input::hlt_Keyboard>* pKeyboardCPool = m_pECS->GetComponent<hlt_Component::hlt_Input::hlt_Keyboard>();

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

void hlt_System::hlt_Input::hlt_Keyboard::SyncKeySize(hlt_Component::hlt_Input::hlt_Keyboard& comp)
{
	size_t maxSize = (std::max)({
		comp.isKey.size(),
		comp.isDown.size(),
		comp.isUp.size()
		});
	size_t keysSize = comp.keys.size();

	if (maxSize != keysSize)
	{
		comp.isKey.resize(keysSize);
		comp.isDown.resize(keysSize);
		comp.isUp.resize(keysSize);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void hlt_System::hlt_Input::hlt_Mouse::Update()
{
	::hlt_Input::MouseInput& mouse = ::hlt_Input::MouseInput ::GetInstance();
	mouse.Update();
}

void hlt_System::hlt_Input::hlt_Mouse::SyncKeySize(hlt_Component::hlt_Input::hlt_Mouse& comp)
{
}
