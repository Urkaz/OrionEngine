#pragma once

#ifdef AE_PLATFORM_WINDOWS

extern ArcEngine::Application* ArcEngine::CreateApplication();

int main()
{
    ArcEngine::Log::Init();

    auto* app = ArcEngine::CreateApplication();
    app->Run();
    delete app;

    return 0;
}

#endif
