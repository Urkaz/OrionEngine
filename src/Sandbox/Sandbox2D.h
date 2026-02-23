#pragma once

#include <Orion.h>

class Sandbox2D : public Orion::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Orion::Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Orion::Event& e) override;

private:
    Orion::OrthographicCameraController m_CameraController;

    // Temp
    Orion::Ref<Orion::VertexArray> m_SquareVA;
    Orion::Ref<Orion::Shader> m_FlatColorShader;

    Orion::Ref<Orion::Texture2D> m_CheckerboardTexture;

    glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};
};
