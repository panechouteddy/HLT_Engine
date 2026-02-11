#pragma once
#include "hlt_ECS.h"

namespace hlt_System
{
	class hlt_SystemClass
	{
	protected:
		hlt_ECS* m_pECS;

	public:
		hlt_SystemClass(hlt_ECS* ecs) : m_pECS(ecs) { ; }

		virtual void Update(float dt) = 0;
	};
}