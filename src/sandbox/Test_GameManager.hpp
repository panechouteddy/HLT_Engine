#include "pch.h"

inline void Test_GameManager()
{
	hlt_GameManager& gm = HLT_GAMEMANAGER;

	std::vector<int> entityID;
	int playerID = gm.CreateEntity();
	int otherID = gm.CreateEntity();
	entityID.push_back(otherID);
	entityID.push_back(playerID);

	gm.GetECS()->AddComponent<hlt_Component::Transform3D>(playerID);
	hlt_Component::hlt_Input::hlt_Keyboard* keyInput = gm.GetECS()->AddComponent<hlt_Component::hlt_Input::hlt_Keyboard>(playerID);
	hlt_Component::BoxCollider* playerBox = gm.GetECS()->AddComponent<hlt_Component::BoxCollider>(playerID);
	hlt_Component::ConstantMove* playerMove = gm.GetECS()->AddComponent<hlt_Component::ConstantMove>(playerID);
	playerMove->move = 1000.f;
	playerMove->dir = DirectX::XMFLOAT3(1.f, 0.f, 0.f);

	gm.GetECS()->AddComponent<hlt_Component::Transform3D>(otherID);
	hlt_Component::BoxCollider* otherBox = gm.GetECS()->AddComponent<hlt_Component::BoxCollider>(otherID);

	gm.GetECS()->AddSystem<hlt_System::BoxCollider>();
	gm.GetECS()->AddSystem<hlt_System::ConstantMove>();

	gm.Run();
}