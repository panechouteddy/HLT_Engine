#include "pch.h"
#include "hlt_CMoveSystem.h"

#include "hlt_CMoveComponent.h"
#include "hlt_Transform3D.h"

void hlt_CMoveSystem::Update(float dt)
{
    {
        hlt_ECS::ComponentPool<hlt_Transform3D>* transforms = m_pECS->GetComponent<hlt_Transform3D>();
        hlt_ECS::ComponentPool<hlt_CMoveComponent>* cMoves = m_pECS->GetComponent<hlt_CMoveComponent>();

        std::vector<int>& cMovesOwners = cMoves->GetComponentOwnersID();

        for (int i = 0; i < cMovesOwners.size(); i++)
        {
            int entityID = cMovesOwners[i];

            if (transforms->entityID[entityID] == MISS_COMPONENT)
                continue;

            XMFLOAT3 entityPos = transforms->component[transforms->entityID[entityID]].pos;
            hlt_CMoveComponent entityMove = cMoves->component[cMoves->entityID[entityID]];

            XMFLOAT3 toMove = { entityMove.move * entityMove.dir.x, entityMove.move * entityMove.dir.y, entityMove.move * entityMove.dir.z };
            XMFLOAT3 newPos = { entityPos.x + toMove.x * dt, entityPos.y + toMove.y * dt, entityPos.z + toMove.z * dt };

            transforms->component[transforms->entityID[entityID]].pos = newPos;
            std::cout << newPos.x << "|" << newPos.y << "|" << newPos.z << std::endl;
        }
    }
}
