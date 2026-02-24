#include <Orion.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

class ExampleLayer : public Orion::Layer
{
public:
    ExampleLayer();
    virtual ~ExampleLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Orion::Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Orion::Event& e) override;

private:
    Orion::ShaderLibrary m_ShaderLibrary;
    Orion::Ref<Orion::VertexArray> m_SquareVA;

    Orion::Ref<Orion::Texture2D> m_Texture, m_TextureLogo;

    Orion::OrthographicCameraController m_CameraController;

    glm::vec3 m_SquareColor = {0.2f, 0.3f, 0.8f};
};
