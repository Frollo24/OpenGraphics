#include "Application.h"

#include "Logger.h"

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
    }

    void Application::Shutdown()
    {
        WindowSystem::Destroy(m_Window);
        WindowSystem::Shutdown();
        Logger::Shutdown();
    }
}
