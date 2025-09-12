#pragma once

#ifdef EMBER_PLATFORM_WINDOWS

extern Ember::Application *Ember::CreateApplication();

int main()
{
    auto *app = Ember::CreateApplication();
    app->Run();
    delete app;

    return 0;
}

#endif