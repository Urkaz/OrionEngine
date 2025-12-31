#include <Orion.h>

#include <imgui.h>

class ExampleLayer : public Orion::Layer
{
public:
    ExampleLayer() : Layer("Example") {}

    void OnImguiRender() override
    {
        ImGui::Begin("Hello, ImGui!");
        ImGui::Text("This is some example text.");
        ImGui::End();
    }
};

class Sandbox : public Orion::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    };
    ~Sandbox() override = default;
};

Orion::Application* Orion::CreateApplication()
{
    return new Sandbox();
}
