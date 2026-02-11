#include "pch.h"

inline void Test_GameManager()
{
	GameManager& gm = GameManager::GetInstance();

	std::vector<int> entityID;
	int playerID = gm.CreateEntity();
	entityID.push_back(playerID);

	gm.GetECS()->AddComponent<hlt_Component::Transform3D>(playerID);
	hlt_Component::hlt_Input::hlt_Keyboard* keyInput = gm.GetECS()->AddComponent<hlt_Component::hlt_Input::hlt_Keyboard>(playerID);

}