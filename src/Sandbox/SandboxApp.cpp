#include <OrionEngine.h>

class Sandbox : public OrionEngine::Application
{
public:
    Sandbox()           = default;
    ~Sandbox() override = default;
};

OrionEngine::Application* OrionEngine::CreateApplication() { return new Sandbox(); }
