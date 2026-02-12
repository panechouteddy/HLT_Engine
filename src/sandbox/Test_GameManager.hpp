#include "pch.h"

inline void Test_GameManager()
{
	hlt_GameManager& gm = HLT_GAMEMANAGER;

	std::vector<int> entityID;
	int playerID = gm.CreateEntity();
	entityID.push_back(playerID);

	gm.GetECS()->AddComponent<hlt_Component::Transform3D>(playerID);
	hlt_Component::hlt_Input::hlt_Keyboard* keyInput = gm.GetECS()->AddComponent<hlt_Component::hlt_Input::hlt_Keyboard>(playerID);

	gm.Run();
}