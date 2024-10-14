#include "Application.h"

namespace OpenGraphics
{
    void Application::Run()
    {
        while (m_Running)
        {
            this->Update();
            this->Render();
        }
    }
}