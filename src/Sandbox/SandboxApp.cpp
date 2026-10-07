#include <Orion.h>
#include <Orion/EntryPoint.h>

class Sandbox : public Orion::Application
{
public:
    Sandbox()
    {
        //PushLayer(new Sandbox2D());
    };
    ~Sandbox() override = default;
};

Orion::Application* Orion::CreateApplication()
{
    return new Sandbox();
}
