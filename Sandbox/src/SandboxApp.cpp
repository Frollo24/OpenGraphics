#define OG_ENTRY_POINT
#include <OpenEngine.h>
#include <iostream>

using namespace OpenGraphics;

class SandboxApp : public OpenGraphics::Application
{
public:
    SandboxApp() = default;
    ~SandboxApp() = default;

    void Initialize() override {
        WindowSystem::Init();
        m_TestWindow = WindowSystem::Create();
    }

    void Update() override {
        if (m_TestWindow->ShouldClose())
            Quit();
    }

    void Render() override {
        m_TestWindow->SwapBuffers();
    }

    void Shutdown() override {
        WindowSystem::Destroy(m_TestWindow);
        WindowSystem::Shutdown();
    }

private:
    Window* m_TestWindow = nullptr;
};

OpenGraphics::Application* OpenGraphics::CreateApplication() {
    return new SandboxApp();
}
