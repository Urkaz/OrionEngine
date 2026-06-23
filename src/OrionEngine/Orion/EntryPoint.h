#pragma once

#include "Orion/Core/Core.h"

#ifdef OE_PLATFORM_WINDOWS

extern Orion::Application* Orion::CreateApplication();

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    Orion::InitializeCore();

    OE_PROFILE_BEGIN_SESSION("Startup", "OrionEngine-Startup.json");
    Orion::Application* app = Orion::CreateApplication();
    OE_CORE_ASSERT(app, "Client Application is null!");
    OE_PROFILE_END_SESSION();

    OE_PROFILE_BEGIN_SESSION("Runtime", "OrionEngine-Runtime.json");
    app->Run();
    OE_PROFILE_END_SESSION();

    OE_PROFILE_BEGIN_SESSION("Shutdown", "OrionEngine-Shutdown.json");
    delete  app;
    Orion::ShutdownCore();
    OE_PROFILE_END_SESSION();

    return 0;
}

#endif
