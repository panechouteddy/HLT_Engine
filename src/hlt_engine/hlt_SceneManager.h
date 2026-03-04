#pragma once

struct hlt_Scene
{
	int ID = -1;
	std::vector<int> activeEntityID;
};


class hlt_SceneManager
{
private:
	std::unordered_map<int, hlt_Scene*> m_pScenes;

public:
	hlt_SceneManager() = default;
	~hlt_SceneManager() = default;

	int AddScene(hlt_Scene* scene);

	hlt_Scene* RemoveScene(int sceneID);
	hlt_Scene* RemoveScene(hlt_Scene* scene);

	void DeleteScene(int sceneID);
	void DeleteScene(hlt_Scene* scene);

	hlt_Scene* GetScene(int sceneID);

	void LoadScene(int sceneID, hlt_ECS* pECS);
	void LoadScene(hlt_Scene* scene, hlt_ECS* pECS);

	void UnloadScene(int sceneID, hlt_ECS* pECS);
	void UnloadScene(hlt_Scene* scene, hlt_ECS* pECS);
};