#include <Orion.h>

class ExampleLayer : public OrionEngine::Layer
{
public:
    ExampleLayer() : Layer("Example") {}

    void OnUpdate() override
    {
        //OE_LOG(info, "ExampleLayer::OnUpdate called");
    }

    void OnEvent(OrionEngine::Event& event) override
    {
        //OE_LOG(trace, "ExampleLayer::OnEvent {0} called", event);
    }
};

class Sandbox : public OrionEngine::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
        PushOverlay(new OrionEngine::ImGuiLayer());
    };
    ~Sandbox() override = default;
};

OrionEngine::Application* OrionEngine::CreateApplication()
{
    return new Sandbox();
}
