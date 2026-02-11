#include "pch.h"
#include "hlt_ConstantMoveSystem.h"

#include "hlt_ConstantMoveComponent.h"
#include "hlt_Transform3DComponent.h"

void hlt_System::ConstantMove::Update()
{
    {
        hlt_ECS::ComponentPool<hlt_Component::Transform3D>* transforms = m_pECS->GetComponent<hlt_Component::Transform3D>();
        hlt_ECS::ComponentPool<hlt_Component::ConstantMove>* cMoves = m_pECS->GetComponent<hlt_Component::ConstantMove>();

        std::vector<int>& cMovesOwners = cMoves->GetComponentOwnersID();
        
        // dt à implémenter

        for (int i = 0; i < cMovesOwners.size(); i++)
        {
            int entityID = cMovesOwners[i];

            if (transforms->entityID[entityID] == MISS_COMPONENT)
                continue;

            XMFLOAT3 entityPos = transforms->component[transforms->entityID[entityID]].pos;
            hlt_Component::ConstantMove entityMove = cMoves->component[cMoves->entityID[entityID]];

            XMFLOAT3 toMove = { entityMove.move * entityMove.dir.x, entityMove.move * entityMove.dir.y, entityMove.move * entityMove.dir.z };
            XMFLOAT3 newPos = { entityPos.x + toMove.x * dt, entityPos.y + toMove.y * dt, entityPos.z + toMove.z * dt };

            transforms->component[transforms->entityID[entityID]].pos = newPos;
            std::cout << newPos.x << "|" << newPos.y << "|" << newPos.z << std::endl;
        }
    }
}

