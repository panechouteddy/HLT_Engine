#pragma once
#include "hlt_System.h"

namespace hlt_System
{
	class Hierarchy : public hlt_SystemClass
	{
	public:
		Hierarchy(hlt_ECS* ecs) : hlt_SystemClass(ecs) { ; }

		// TO DO
		void Update() override { ; }

	/*private:
		std::vector<std::vector<std::pair<int, int>>> GetHierarchyDepth(hlt_ECS::ComponentPool<hlt_Component::Hierarchy>* hierarchy);*/
	};
}

