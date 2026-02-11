#pragma once
#include "hlt_System.h"

namespace hlt_System
{
	class hlt_CMoveSystem : public hlt_SystemClass
	{
	public:
		hlt_CMoveSystem(hlt_ECS* ecs) : hlt_SystemClass(ecs) { ; }

		void Update(float dt) override;
	};
}

