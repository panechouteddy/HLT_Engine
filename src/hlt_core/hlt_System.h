#pragma once
#include "hlt_ECS.h"

class hlt_System
{
protected:
	hlt_ECS* m_pECS;

public:
	hlt_System(hlt_ECS* ecs) : m_pECS(ecs) { ; }

	virtual void Update(float dt) = 0;
};