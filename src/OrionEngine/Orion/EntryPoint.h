#pragma once

#ifdef OE_PLATFORM_WINDOWS

extern Orion::Application* Orion::CreateApplication();

int main()
{
    Orion::Log::Init();

    auto* app = Orion::CreateApplication();
    app->Run();
    delete app;

    return 0;
}

#endif
