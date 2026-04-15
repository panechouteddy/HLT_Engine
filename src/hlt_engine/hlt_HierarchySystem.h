#pragma once
#include "hlt_System.h"

namespace hlt_System
{
	class Hierarchy : public hlt_SystemClass
	{
	public:
		static const int ID = 7;

	public:
		Hierarchy(hlt_ECS* ecs) : hlt_SystemClass(ecs) { ; }

		void Update() override;

	private:
		std::vector<std::vector<int>> SparseHierarchyDepth(hlt_ECS::ComponentPool<hlt_Component::Hierarchy>* rawHierarchy);

		int GetComponentDepth(hlt_Component::Hierarchy& component);
	};
}