#pragma once

#include "OpenEngine/Core/Base.h"
#include "OpenEngine/Windowing/WindowSystem.h"

namespace OpenGraphics
{
    class OPEN_API Application
    {
    public:
        Application();
        virtual ~Application();

        void OnCreate();
        void Run();
        void OnDestroy();

        void Quit() { m_Running = false; }

    protected:
        virtual void Load() = 0;
        virtual void Update() = 0;
        virtual void Render() = 0;
        virtual void Cleanup() = 0;

        [[nodiscard]] const Window* GetWindow() const { return m_Window; }

    private:
        void Initialize();
        void Shutdown();

    private:
        inline static Application* s_Instance = nullptr;
        Window* m_Window = nullptr;
        bool m_Running = true;
    };

    Application* CreateApplication();
}
