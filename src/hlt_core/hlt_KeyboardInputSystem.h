#pragma once
#include "hlt_System.h"
#include "hlt_KeyboardInputComponent.h"

namespace hlt_System
{
	class hlt_KeyboardInputSystem : public hlt_SystemClass
	{
	public:
		hlt_KeyboardInputSystem(hlt_ECS* ecs) : hlt_SystemClass(ecs) { ; }

		void Update(float dt) override;

	private:
		void SyncKeySize(hlt_Component::hlt_KeyboardInputComponent& comp);
	};
}

