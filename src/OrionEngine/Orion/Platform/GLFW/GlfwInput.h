#pragma once

#include "Orion/Core/Input.h"

namespace Orion
{
    class GlfwInput final : public Input
    {
    protected:
        bool IsKeyPressedImpl(KeyCode key) override;
        bool IsMouseButtonPressedImpl(MouseCode button) override;
        float GetMouseXImpl() override;
        float GetMouseYImpl() override;
        std::pair<float, float> GetMousePositionImpl() override;
        void SetCursorModeImpl(CursorMode mode) override;
    };
} // namespace Orion
