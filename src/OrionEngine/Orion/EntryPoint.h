#pragma once

#ifdef OE_PLATFORM_WINDOWS

extern OrionEngine::Application* OrionEngine::CreateApplication();

int main()
{
    OrionEngine::Log::Init();

    auto* app = OrionEngine::CreateApplication();
    app->Run();
    delete app;

    return 0;
}

#endif
