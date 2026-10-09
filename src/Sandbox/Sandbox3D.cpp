#include "Orion/Renderer/Buffer.h"
#include "Orion/Renderer/RenderCommand.h"
#include "Orion/Renderer/Renderer.h"
#include "Orion/Renderer/VertexArray.h"
#include "Sandbox3D.h"

#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

Sandbox3D::Sandbox3D() : Layer("Sandbox3D"), m_Camera(45.0f, 1280.f / 720.f, 0.1f, 1000.0f) {}

void Sandbox3D::OnAttach()
{
    // Each face: bottom-left, bottom-right, top-right, top-left as seen from OUTSIDE the cube -> counter-clockwise -> front face.
    float vertices[4 /*vertex per face*/ * (3 /*position*/ + 3 /*color*/) * 6 /*faces*/] = {
        // clang-format off
        // +Z (front) red
        -0.5f,   -0.5f,   0.5f,    0.9f,   0.2f,   0.2f,
        0.5f,    -0.5f,   0.5f,    0.9f,   0.2f,  0.2f,
        0.5f,   0.5f,   0.5f,   0.9f,  0.2f,  0.2f,
        -0.5f,  0.5f,   0.5f,   0.9f,  0.2f,  0.2f,
        // -Z (back) cyan
        0.5f,   -0.5f,  -0.5f,  0.2f,  0.8f,  0.9f,
        -0.5f,  -0.5f,  -0.5f,  0.2f,  0.8f,  0.9f,
        -0.5f,  0.5f,   -0.5f,  0.2f,  0.8f,  0.9f,
        0.5f,   0.5f,   -0.5f,  0.2f,  0.8f,  0.9f,
        // +X (right) green
        0.5f,   -0.5f,  0.5f,   0.2f,  0.9f,  0.3f,
        0.5f,   -0.5f,  -0.5f,  0.2f,  0.9f,  0.3f,
        0.5f,   0.5f,   -0.5f,  0.2f,  0.9f,  0.3f,
        0.5f,   0.5f,   0.5f,   0.2f,  0.9f,  0.3f,
        // -X (left) magenta
        -0.5f,  -0.5f,  -0.5f,  0.9f,  0.2f,  0.9f,
        -0.5f,  -0.5f,  0.5f,   0.9f,  0.2f,  0.9f,
        -0.5f,  0.5f,   0.5f,   0.9f,  0.2f,  0.9f,
        -0.5f,  0.5f,   -0.5f,  0.9f,  0.2f,  0.9f,
        // +Y (top) blue
        -0.5f,  0.5f,   0.5f,   0.2f,  0.4f, 0.9f,
        0.5f,  0.5f,  0.5f,  0.2f, 0.4f, 0.9f,
        0.5f,  0.5f,  -0.5f, 0.2f, 0.4f, 0.9f,
        -0.5f, 0.5f,  -0.5f, 0.2f, 0.4f, 0.9f,
        // -Y (bottom) yellow
        -0.5f, -0.5f, -0.5f, 0.9f, 0.9f, 0.2f,
        0.5f,  -0.5f, -0.5f, 0.9f, 0.9f, 0.2f,
        0.5f,  -0.5f, 0.5f,  0.9f, 0.9f, 0.2f,
        -0.5f, -0.5f, 0.5f,  0.9f, 0.9f, 0.2f,
        // clang-format on
    };

    uint32_t indices[6 /*indices*/ * 6 /*faces*/];
    for (uint32_t face = 0; face < 6; ++face)
    {
        const uint32_t v = face * 4;
        const uint32_t i = face * 6;

        // Two triangles per face, both keep the counter-clockwise order
        indices[i + 0] = v + 0;
        indices[i + 1] = v + 1;
        indices[i + 2] = v + 2;
        indices[i + 3] = v + 2;
        indices[i + 4] = v + 3;
        indices[i + 5] = v + 0;
    }

    m_CubeVA = Orion::VertexArray::Create();

    Orion::Ref<Orion::VertexBuffer> vb = Orion::VertexBuffer::Create(vertices, sizeof(vertices));
    vb->SetLayout({{Orion::ShaderDataType::Float3, "a_Position"}, {Orion::ShaderDataType::Float3, "a_Color"}});
    m_CubeVA->AddVertexBuffer(vb);

    Orion::Ref<Orion::IndexBuffer> ib = Orion::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
    m_CubeVA->SetIndexBuffer(ib);

    m_CubeShader = Orion::Shader::Create("assets/shaders/Cube.glsl");
}

void Sandbox3D::OnUpdate(Orion::Timestep ts)
{
    OE_PROFILE_FUNCTION();

    m_FrameTime = ts.GetSeconds();
    m_Time += ts.GetSeconds();
    m_Camera.OnUpdate(ts);

    Orion::RenderCommand::SetDepthTest(m_DepthTest);
    Orion::RenderCommand::SetFaceCulling(m_FaceCulling);
    Orion::RenderCommand::SetClearColor({0.08f, 0.08f, 0.1f, 1.0f});
    Orion::RenderCommand::Clear();

    Orion::Renderer::BeginScene(m_Camera, m_Camera.GetViewMatrix());

    const glm::mat4 cubeTransform = glm::translate(glm::mat4(1.0f), {0.0f, 0.5f, 0.0f}) *
                                    glm::rotate(glm::mat4(1.0f), m_Time, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f))) *
                                    glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
    Orion::Renderer::Submit(m_CubeShader, m_CubeVA, cubeTransform);

    for (int z = 10; z >= -10; --z)
    {
        for (int x = -10; x <= 10; ++x)
        {
            const glm::vec3 position = {static_cast<float>(x) * 2.0f, -1.0f, static_cast<float>(z) * 2.0f};
            const glm::mat4 transform =
                glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {1.8f, 0.1f, 1.8f});
            Orion::Renderer::Submit(m_CubeShader, m_CubeVA, transform);
        }
    }

    Orion::Renderer::EndScene();
}

void Sandbox3D::OnImGuiRender()
{
    ImGui::Begin("Camera");

    ImGui::Text("%.2f ms (%.0f FPS)", m_FrameTime * 1000.0f, 1.0f / m_FrameTime);

    const glm::vec3& p = m_Camera.GetPosition();
    ImGui::Text("Position: %.2f, %.2f, %.2f", p.x, p.y, p.z);
    ImGui::Text("Yaw: %.1f  Pitch: %.1f", m_Camera.GetYaw(), m_Camera.GetPitch());
    ImGui::Text("Move speed: %.2f u/s (mouse wheel)", m_Camera.GetMoveSpeed());

    float fov = m_Camera.GetFOV();
    if (ImGui::SliderFloat("FOV", &fov, 10.0f, 120.0f))
        m_Camera.SetFOV(fov);

    float nearClip = m_Camera.GetNearClip();
    float farClip  = m_Camera.GetFarClip();
    bool changed   = ImGui::DragFloat("Near", &nearClip, 0.001f, 0.0001f, 10.0f, "%.4f", ImGuiSliderFlags_Logarithmic);
    changed |= ImGui::DragFloat("Far", &farClip, 1.0f, 1.0f, 100000.0f, "%.1f", ImGuiSliderFlags_Logarithmic);
    if (changed && nearClip < farClip)
        m_Camera.SetClipPlanes(nearClip, farClip);

    ImGui::Separator();
    ImGui::Checkbox("Depth test", &m_DepthTest);
    ImGui::Checkbox("Back-face culling", &m_FaceCulling);

    ImGui::End();
}

void Sandbox3D::OnEvent(Orion::Event& e)
{
    m_Camera.OnEvent(e);
}
