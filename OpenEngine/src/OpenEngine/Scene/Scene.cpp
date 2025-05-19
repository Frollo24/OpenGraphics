#include "ogpch.h"
#include "Scene.h"

namespace OpenGraphics
{
    Scene::Scene(const std::string& name)
        : m_SceneName(name)
    {
    }

    Scene::~Scene()
    {
        RemoveAllGameObjects();
    }

    void Scene::AddGameObject(GameObject* gameObject)
    {
        m_GameObjects.push_back(gameObject);
        gameObject->OnCreate();
    }

    void Scene::RemoveGameObject(const GameObject* gameObject)
    {
        auto it = std::find(m_GameObjects.begin(), m_GameObjects.end(), gameObject);
        if (it != m_GameObjects.end())
            m_GameObjects.erase(it);

        delete gameObject;
    }

    void Scene::RemoveAllGameObjects()
    {
        while (!m_GameObjects.empty())
            RemoveGameObject(m_GameObjects[0]);

        m_GameObjects.clear();
    }

    void Scene::OnCreate()
    {
        for (size_t i = 0; i < m_GameObjects.size(); i++)
            m_GameObjects[i]->OnCreate();
    }

    void Scene::OnUpdate()
    {
        for (size_t i = 0; i < m_GameObjects.size(); i++)
            m_GameObjects[i]->OnUpdate();
    }

    void Scene::OnRender()
    {
        for (size_t i = 0; i < m_GameObjects.size(); i++)
            m_GameObjects[i]->OnRender();
    }

    void Scene::OnDestroy()
    {
        for (size_t i = 0; i < m_GameObjects.size(); i++)
            m_GameObjects[i]->OnDestroy();
    }
}
