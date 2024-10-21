#define OG_ENTRY_POINT
#include <OpenEngine.h>
#include <iostream>

using namespace OpenGraphics;

class SandboxApp : public OpenGraphics::Application
{
public:
    SandboxApp() = default;
    ~SandboxApp() = default;

    void Load() override {
        std::cout << "Resource loading..." << std::endl;
    }

    void Update() override {
        if (GetWindow()->ShouldClose())
            Quit();
    }

    void Render() override {
        RenderCommand::BeginFrame();
        RenderCommand::EndFrame();
        GetWindow()->SwapBuffers();
    }

    void Cleanup() override {
        std::cout << "Resource cleanup..." << std::endl;
    }
};

OpenGraphics::Application* OpenGraphics::CreateApplication() {
    return new SandboxApp();
}
