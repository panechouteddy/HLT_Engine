#include "pch.h"

void hlt_System::Hierarchy::Update()
{
	hlt_ECS::ComponentPool<hlt_Component::Hierarchy>* hierarchys = m_pECS->GetComponent<hlt_Component::Hierarchy>();
	std::vector<std::vector<int>> sortedHierarchys = SparseHierarchyDepth(hierarchys);

	int currentSlot = 0;

	for (int depthLevel = 0; depthLevel < sortedHierarchys.size(); depthLevel++)
	{
		for (int entityID : sortedHierarchys[depthLevel])
		{
			// On récupère l'index actuel du composant pour cette entité
			int actualIndex = hierarchys->entityID[entityID];

			if (actualIndex != currentSlot)
			{
				// On doit swapper le composant à actualIndex vers currentSlot
				// pour que le pool soit trié par profondeur

				// Swap dans le pool Hierarchy
				std::swap(hierarchys->component[actualIndex], hierarchys->component[currentSlot]);
				std::swap(hierarchys->componentOwnerID[actualIndex], hierarchys->componentOwnerID[currentSlot]);

				// Mise à jour de la table de redirection (entityID)
				hierarchys->entityID[hierarchys->componentOwnerID[actualIndex]] = actualIndex;
				hierarchys->entityID[hierarchys->componentOwnerID[currentSlot]] = currentSlot;
			}
			currentSlot++;
		}
	}
}

std::vector<std::vector<int>> hlt_System::Hierarchy::SparseHierarchyDepth(hlt_ECS::ComponentPool<hlt_Component::Hierarchy>* rawHierarchy)
{
	std::vector<std::vector<int>> depth;

	for (int i = 0; i < rawHierarchy->componentOwnerID.size(); i++)
	{
		rawHierarchy->component[i]->isRefreshed = false;
		int componentDepth = GetComponentDepth(*rawHierarchy->component[i]);

		if (depth.size() <= componentDepth)
			depth.push_back(std::vector<int>());

		depth[componentDepth].push_back(rawHierarchy->componentOwnerID[i]);
	}

	return depth;
}

int hlt_System::Hierarchy::GetComponentDepth(hlt_Component::Hierarchy& component)
{
	if (component.isRefreshed)
		return component.hierarchyDepth;

	hlt_Component::Hierarchy* parent = m_pECS->GetComponent<hlt_Component::Hierarchy>(component.parentID);
	if (component.parentID == NO_PARENT || parent == nullptr)
		return 0;

	int depth = GetComponentDepth(*parent) + 1;
	component.isRefreshed = true;
	component.hierarchyDepth = depth;
	return depth;
}
