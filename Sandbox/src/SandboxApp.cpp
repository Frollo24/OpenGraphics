#define OG_ENTRY_POINT
#include <OpenEngine.h>
#include <iostream>

class SandboxApp : public OpenGraphics::Application
{
public:
    SandboxApp() = default;
    ~SandboxApp() = default;

    void Initialize() override {
        std::cout << "Initialize" << std::endl;
    }

    void Update() override {
        std::cout << "Update" << std::endl;
    }

    void Render() override {
        std::cout << "Render" << std::endl;
    }

    void Shutdown() override {
        std::cout << "Shutdown" << std::endl;
    }
};

OpenGraphics::Application* OpenGraphics::CreateApplication() {
    return new SandboxApp();
}
