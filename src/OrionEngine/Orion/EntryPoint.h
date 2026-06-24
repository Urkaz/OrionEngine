#pragma once

#include "Orion/Core/Core.h"

#ifdef OE_PLATFORM_WINDOWS

extern Orion::Application* Orion::CreateApplication();

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    OE_PROFILE_WAIT_FOR_CONNECTION();

    Orion::InitializeCore();

    Orion::Application* app = Orion::CreateApplication();
    OE_CORE_ASSERT(app, "Client Application is null!");

    app->Run();

    delete  app;
    Orion::ShutdownCore();

    return 0;
}

#endif
