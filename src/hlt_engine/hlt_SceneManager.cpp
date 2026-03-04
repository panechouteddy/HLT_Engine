#include "pch.h"


int hlt_SceneManager::AddScene(hlt_Scene* scene)
{
    m_pScenes[scene->ID] = scene;
    return scene->ID;
}

hlt_Scene* hlt_SceneManager::RemoveScene(int sceneID)
{
    if (m_pScenes.contains(sceneID) == false) return nullptr;

    hlt_Scene* pScene = m_pScenes[sceneID];
    m_pScenes.erase(sceneID);
    return pScene;
}

hlt_Scene* hlt_SceneManager::RemoveScene(hlt_Scene* scene)
{
    return RemoveScene(scene->ID);
}

void hlt_SceneManager::DeleteScene(int sceneID)
{
    delete RemoveScene(sceneID);
}

void hlt_SceneManager::DeleteScene(hlt_Scene* scene)
{
    delete RemoveScene(scene);
}

hlt_Scene* hlt_SceneManager::GetScene(int sceneID)
{
    if (m_pScenes.contains(sceneID) == false) return nullptr;

    return m_pScenes[sceneID];
}

void hlt_SceneManager::LoadScene(int sceneID, hlt_ECS* pECS)
{
    hlt_Scene* pScene = GetScene(sceneID);

    if (pScene == nullptr) return;

    for (int& eID : pScene->activeEntityID)
    {
        // ENTITY->SETACTIVE(TRUE)
    }
}

void hlt_SceneManager::LoadScene(hlt_Scene* scene, hlt_ECS* pECS)
{
    if (scene == nullptr) return;

    for (int& eID : scene->activeEntityID)
    {
        // ENTITY->SETACTIVE(TRUE)
    }
}

void hlt_SceneManager::UnloadScene(int sceneID, hlt_ECS* pECS)
{
    hlt_Scene* pScene = GetScene(sceneID);

    if (pScene == nullptr) return;

    for (int& eID : pScene->activeEntityID)
    {
        // ENTITY->SETACTIVE(FALSE)
    }
}

void hlt_SceneManager::UnloadScene(hlt_Scene* scene, hlt_ECS* pECS)
{
    if (scene == nullptr) return;

    for (int& eID : scene->activeEntityID)
    {
        // ENTITY->SETACTIVE(FALSE)
    }
}
