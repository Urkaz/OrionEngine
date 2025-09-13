#pragma once

#include "Core.h"

namespace ArcEngine
{
    class ARCENGINE_API Application
    {
    public:
        Application()          = default;
        virtual ~Application() = default;

        void Run();
    };

    // To be defined in client
    Application* CreateApplication();
} // namespace ArcEngine
