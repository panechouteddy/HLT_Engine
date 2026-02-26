#include "pch.h"

//void hlt_System::Hierarchy::Update()
//{
//	hlt_ECS::ComponentPool<hlt_Component::Hierarchy>* hierarchy = m_pECS->GetComponent<hlt_Component::Hierarchy>();
//	hlt_ECS::ComponentPool<hlt_Component::Transform3D>* transforms = m_pECS->GetComponent<hlt_Component::Transform3D>();
//
//	std::vector<std::vector<std::pair<int, int>>> depths = GetHierarchyDepth(hierarchy);
//
//	for (int depthLevel = 1; depthLevel < depths.size(); depthLevel++)
//	{
//		for (int i = 0; i < depths[depthLevel].size(); i++)
//		{
//			int eID = depths[depthLevel][i].first;
//			int parentID = depths[depthLevel][i].second;
//
//			hlt_Component::Transform3D& transform = transforms->component[]
//		}
//	}
//}
//
//std::vector<std::vector<std::pair<int, int>>> hlt_System::Hierarchy::GetHierarchyDepth(hlt_ECS::ComponentPool<hlt_Component::Hierarchy>* hierarchy)
//{
//	std::vector<std::vector<std::pair<int, int>>> depthVector;
//
//	for (int i = 0; i < hierarchy->componentOwnerID.size(); i++)
//	{
//		hlt_Component::Hierarchy& current = hierarchy->component[i];
//
//		int depth = 0;
//		hlt_Component::Hierarchy& curr = current;
//		while (curr.parentID != NO_PARENT)
//		{
//			depth++;
//			curr = *hierarchy->Get(curr.parentID);
//		}
//
//		if (depthVector.size() <= depth)
//		{
//			depthVector.resize(depth + 1);
//		}
//		depthVector[depth].push_back(std::pair<int, int>(i, current.parentID));
//	}
//	return depthVector;
//}
