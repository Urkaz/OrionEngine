#include <ArcEngine.h>

class Sandbox : public ArcEngine::Application
{
public:
    Sandbox()           = default;
    ~Sandbox() override = default;
};

ArcEngine::Application* ArcEngine::CreateApplication() { return new Sandbox(); }
