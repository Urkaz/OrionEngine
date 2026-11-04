#pragma once

#include "Orion/Core/Application.h"
#include "Orion/Core/Assert.h"
#include "Orion/Core/Core.h"
#include "Orion/Profiling/Profiling.h"

#if defined(OE_PLATFORM_WINDOWS) || defined(OE_PLATFORM_LINUX)

extern Orion::Scope<Orion::Application> Orion::CreateApplication();

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    OE_PROFILE_WAIT_FOR_CONNECTION();

    Orion::InitializeCore();

    Orion::Scope<Orion::Application> app = Orion::CreateApplication();
    OE_CORE_ASSERT(app, "Client Application is null!");

    app->Run();

    Orion::ShutdownCore();

    return 0;
}

#else
    #error "No entrypoint defined for this platform!"
#endif
