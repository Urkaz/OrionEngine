#pragma once

#ifdef OE_PLATFORM_WINDOWS

extern Orion::Application* Orion::CreateApplication();

int main()
{
    Orion::InitializeCore();

    Orion::Application* app = Orion::CreateApplication();
    OE_CORE_ASSERT(app, "Client Application is null!");
    app->Run();
    delete app;

    Orion::ShutdownCore();

    return 0;
}

#endif
