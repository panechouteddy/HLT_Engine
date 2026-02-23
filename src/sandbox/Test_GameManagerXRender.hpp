#include "pch.h"

inline void Test_GameManagerXRender()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	HLT::Run<App>();

	hlt_GameManager& gm = HLT_GAMEMANAGER;

	gm.Start();

	std::vector<int> entityID;
	int playerID = gm.CreateEntity();
	entityID.push_back(playerID);

	
	gm.GetECS()->AddComponent<hlt_Component::Transform3D>(playerID);
	hlt_Component::Mesh* playerMesh = gm.GetECS()->AddComponent<hlt_Component::Mesh>(playerID);

	playerMesh->mesh.SetMesh("Pyramid");
	gm.AddMesh(&playerMesh->mesh);
	gm.Run();
}