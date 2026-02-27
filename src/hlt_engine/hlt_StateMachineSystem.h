#pragma once

namespace hlt_System
{
	class StateMachine : public hlt_SystemClass
	{
	public:
		static const int ID = 6;

	public:
		StateMachine(hlt_ECS* ecs) : hlt_SystemClass(ecs) { ; }

		void Update() override;
	};
}

