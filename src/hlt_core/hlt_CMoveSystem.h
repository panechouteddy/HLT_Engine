#pragma once
#include "hlt_System.h"

class hlt_CMoveSystem : public hlt_System
{
public:
	hlt_CMoveSystem(hlt_ECS* ecs) : hlt_System(ecs) { ; }

	void Update(float dt) override;
};

