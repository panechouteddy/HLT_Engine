#pragma once
#include "hlt_System.h"

namespace hlt_System
{
	class hlt_RepulseSystem : public hlt_SystemClass
	{
	public:
		static const int ID = 5;

	public:
		hlt_RepulseSystem(hlt_ECS* ecs) : hlt_SystemClass(ecs) { ; }

		void Update() override;
	};
}

