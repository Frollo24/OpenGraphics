#include "ogpch.h"
#include "Application.h"
#include "OpenEngine/Render/RenderSystem.h"

namespace OpenGraphics
{
    Application::Application()
    {
        if (s_Instance)
        {
            delete this;
            return;
        }

        s_Instance = this;
    }

    Application::~Application()
    {
        if (s_Instance == this)
            s_Instance = nullptr;
    }

    void Application::OnCreate()
    {
        Initialize();
        this->Load();
    }

    void Application::Run()
    {
        while (m_Running)
        {
            this->Update();
            this->Render();
        }
    }

    void Application::OnDestroy()
    {
        this->Cleanup();
        Shutdown();
    }

    void Application::Initialize()
    {
        Logger::Init();
        WindowSystem::Init();
        m_Window = WindowSystem::Create();

        RenderSystem::Init();
    }

    void Application::Shutdown()
    {
        RenderSystem::Shutdown();

        WindowSystem::Destroy(m_Window);
        m_Window = nullptr;

        WindowSystem::Shutdown();
        Logger::Shutdown();
    }
}
