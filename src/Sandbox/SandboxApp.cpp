#include <Orion.h>

class ExampleLayer : public OrionEngine::Layer
{
public:
    ExampleLayer() : Layer("Example") {}

    void OnImguiRender() override
    {
        // OE_CORE_LOG(info, "Before ExampleLayer render");
        // ImGui::Begin("Hello, ImGui!");
        // ImGui::Text("This is some example text.");
        // ImGui::End();
    }
};

class Sandbox : public OrionEngine::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    };
    ~Sandbox() override = default;
};

OrionEngine::Application* OrionEngine::CreateApplication()
{
    return new Sandbox();
}
