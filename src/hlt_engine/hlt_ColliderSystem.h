#pragma once
#include "hlt_System.h"

namespace hlt_System
{
	class BoxCollider : public hlt_SystemClass
	{
	public:
		static const int ID = 4;

	public:
		BoxCollider(hlt_ECS* ecs) : hlt_SystemClass(ecs) { ; }

		void Update() override;
	};
}

