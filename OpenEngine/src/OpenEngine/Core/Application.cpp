#include "Application.h"

namespace OpenGraphics
{
    void Application::Run()
    {
        this->Update();
        this->Render();
    }
}