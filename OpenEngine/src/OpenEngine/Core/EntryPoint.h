#pragma once
#include "OpenEngine.h"

extern OpenGraphics::Application* OpenGraphics::CreateApplication(); // To be defined by client

namespace OpenGraphics
{
    int Main(int argc, char** argv)
    {
        auto app = CreateApplication();
        app->Initialize();
        app->Run();
        app->Shutdown();
        delete app;

        return 0;
    }
}

int main(int argc, char** argv)
{
    return OpenGraphics::Main(argc, argv);
}