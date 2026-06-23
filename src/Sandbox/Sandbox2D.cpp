#include "Sandbox2D.h"

#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::OnAttach()
{
    OE_PROFILE_FUNCTION();

    m_CheckerboardTexture = Orion::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
    OE_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Orion::Timestep ts)
{
    OE_PROFILE_FUNCTION();

    // Update
    m_CameraController.OnUpdate(ts);

    // Render
    {
        OE_PROFILE_SCOPE("Renderer Prep");
        Orion::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Orion::RenderCommand::Clear();
    }

    {
        OE_PROFILE_SCOPE("Renderer Draw");
        Orion::Renderer2D::BeginScene(m_CameraController.GetCamera());
        Orion::Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
        Orion::Renderer2D::DrawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, {0.2f, 0.3f, 0.8f, 1.0f});
        Orion::Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {10.0f, 10.0f}, m_CheckerboardTexture);
        Orion::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender()
{
    OE_PROFILE_FUNCTION();

    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Orion::Event& e)
{
    m_CameraController.OnEvent(e);
}
