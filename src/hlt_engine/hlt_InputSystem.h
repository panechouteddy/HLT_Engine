#pragma once
#include "hlt_System.h"
#include "hlt_InputComponent.h"

namespace hlt_System
{
	namespace hlt_Input
	{
		class hlt_Keyboard : public hlt_SystemClass
		{
		public:
			hlt_Keyboard(hlt_ECS* ecs) : hlt_SystemClass(ecs) { ; }

			void Update() override;

		private:
			void SyncKeySize(hlt_Component::hlt_Input::hlt_Keyboard& comp);
		};

		class hlt_Mouse : public hlt_SystemClass
		{
		public:
			hlt_Mouse(hlt_ECS* ecs) : hlt_SystemClass(ecs) { ; }

			void Update() override;

		private:
			void SyncKeySize(hlt_Component::hlt_Input::hlt_Mouse& comp);
		};
	}	
}

