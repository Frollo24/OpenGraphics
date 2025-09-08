#pragma once
#include "OpenEngine.h"

extern OpenGraphics::Application* OpenGraphics::CreateApplication(); // To be defined by client

namespace OpenGraphics
{
    inline int Main(int argc, char** argv)
    {
        const auto app = CreateApplication();
        app->OnCreate();
        app->Run();
        app->OnDestroy();
        delete app;

        return 0;
    }
}

int main(const int argc, char** argv)
{
    return OpenGraphics::Main(argc, argv);
}