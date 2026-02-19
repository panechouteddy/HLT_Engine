#pragma once
#include "hlt_System.h"

namespace hlt_System
{
	class ConstantMove : public hlt_SystemClass
	{
	public:
		ConstantMove(hlt_ECS* ecs) : hlt_SystemClass(ecs) { ; }

		void Update() override;
	};
}

