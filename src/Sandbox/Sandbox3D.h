#pragma once

#include <Orion.h>

class Sandbox3D : public Orion::Layer
{
public:
    Sandbox3D();

    void OnAttach() override;
    void OnUpdate(Orion::Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Orion::Event& e) override;

private:
    void DrawNodeTree(const Orion::MeshSource& source, uint32_t nodeIndex);

private:
    Orion::FlyCamera m_Camera;

    Orion::Ref<Orion::VertexArray> m_CubeVA;
    Orion::Ref<Orion::Shader> m_CubeShader;

    float m_FrameTime = 0.0f;
    float m_Time      = 0.0f;

    // Debug toggles
    bool m_DepthTest   = true;
    bool m_FaceCulling = true;

    Orion::Ref<Orion::MeshRenderData> m_Model;
    Orion::Ref<Orion::Shader> m_MeshShader;
    int m_DebugMode    = 0;
    float m_ModelScale = 1.0f;

    Orion::Ref<Orion::Shader> m_LitShader;
    int m_ShadingMode  = 0;      // 0 lit, 1 debug
    int m_ViewMode     = 0;      // MeshLit.glsl u_ViewMode
    float m_LightYaw   = -35.0f; // degrees, 0 = travelling towards -Z (as FlyCamera)
    float m_LightPitch = -45.0f; // degrees, negative = pointing down
    glm::vec3 m_LightColor{1.0f, 0.96f, 0.9f};
    float m_LightIntensity = 3.0f;
    glm::vec3 m_Ambient{0.03f, 0.03f, 0.04f};
    float m_Shininess        = 32.0f;
    float m_SpecularStrength = 0.5f;
};
