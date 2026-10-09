#pragma once

#include <Orion.h>

class Sandbox3D : public Orion::Layer
{
public:
    Sandbox3D();
    virtual ~Sandbox3D() override = default;

    virtual void OnAttach() override;
    virtual void OnUpdate(Orion::Timestep ts) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Orion::Event& e) override;

private:
    Orion::FlyCamera m_Camera;

    Orion::Ref<Orion::VertexArray> m_CubeVA;
    Orion::Ref<Orion::Shader> m_CubeShader;

    float m_FrameTime = 0.0f;
    float m_Time = 0.0f;

    // Debug toggles
    bool m_DepthTest   = true;
    bool m_FaceCulling = true;
};
