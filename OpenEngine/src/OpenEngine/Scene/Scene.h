#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Scene/GameObject.h"

namespace OpenGraphics
{
    class RenderCamera;

    class OPEN_API Scene final
    {
    public:
        Scene(const std::string& name = "Sample Scene");
        ~Scene();

        void AddGameObject(GameObject* gameObject);
        void RemoveGameObject(const GameObject* gameObject);
        void RemoveAllGameObjects();

        [[nodiscard]] inline const std::vector<GameObject*>& GetGameObjects() const { return m_GameObjects; }
        [[nodiscard]] inline const std::string& GetName() { return m_SceneName; }
        inline void SetName(const std::string& name) { m_SceneName = name; }

        void OnCreate();
        void OnUpdate();
        void OnRender();
        void OnDestroy();

    private:
        std::vector<GameObject*> m_GameObjects;
        std::string m_SceneName;
    };
}
