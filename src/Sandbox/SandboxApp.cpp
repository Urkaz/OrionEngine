#include "Sandbox3D.h"

#include <Orion.h>
#include <Orion/EntryPoint.h>

class Sandbox : public Orion::Application
{
public:
    Sandbox()
    {
#ifdef SANDBOX_SOURCE_ASSETS_DIR
        // Remount assets directory to use assets from the source tree instead of the copied ones
        Orion::FileSystem::Mount("assets", Orion::CreateScope<Orion::DiskMountSource>(SANDBOX_SOURCE_ASSETS_DIR, true));
#endif
        PushLayer(new Sandbox3D());
    };
    ~Sandbox() override = default;
};

Orion::Scope<Orion::Application> Orion::CreateApplication()
{
    return Orion::CreateScope<Sandbox>();
}
