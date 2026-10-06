#pragma once

#include "Orion/Core/Input.h"

namespace Orion
{
    class GlfwInput : public Input
    {
    protected:
        virtual bool IsKeyPressedImpl(KeyCode key) override;
        virtual bool IsMouseButtonPressedImpl(MouseCode button) override;
        virtual float GetMouseXImpl() override;
        virtual float GetMouseYImpl() override;
        virtual std::pair<float, float> GetMousePositionImpl() override;
        virtual void SetCursorModeImpl(CursorMode mode) override;
    };
} // namespace Orion
