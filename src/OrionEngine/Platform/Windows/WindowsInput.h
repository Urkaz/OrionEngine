#pragma once

#include "Orion/Input.h"

namespace Orion
{
    class WindowsInput : public Input
    {
    protected:
        virtual bool IsKeyPressedImpl(KeyCode key) override;
        virtual bool IsMouseButtonPressedImpl(MouseCode button) override;
        virtual float GetMouseXImpl() override;
        virtual float GetMouseYImpl() override;
        virtual std::pair<float, float> GetMousePositionImpl() override;
    };
} // namespace Orion
