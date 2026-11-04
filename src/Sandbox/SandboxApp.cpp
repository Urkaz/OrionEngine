#include <Orion.h>
#include <Orion/EntryPoint.h>

#include "Sandbox3D.h"

class Sandbox : public Orion::Application
{
public:
    Sandbox()
    {
        PushLayer(new Sandbox3D());
    };
    ~Sandbox() override = default;
};

Orion::Scope<Orion::Application> Orion::CreateApplication()
{
    return Orion::CreateScope<Sandbox>();
}
