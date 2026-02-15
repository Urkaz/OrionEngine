#include <Orion.h>

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Orion::Layer
{
public:
    ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
    {
        // SQUARE
        m_SquareVA                  = Orion::VertexArray::Create();
        float squareVertices[5 * 4] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
                                       0.5f,  0.5f,  0.0f, 1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f};
        Orion::Ref<Orion::VertexBuffer> squareVB;
        squareVB                           = Orion::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
        Orion::BufferLayout squareVBLayout = {{Orion::ShaderDataType::Float3, "a_Position"},
                                              {Orion::ShaderDataType::Float2, "a_TexCoord"}};
        squareVB->SetLayout(squareVBLayout);
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
        Orion::Ref<Orion::IndexBuffer> squareIB;
        squareIB = Orion::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
        m_SquareVA->SetIndexBuffer(squareIB);

        // Shaders
        m_FlatColorShader = Orion::Shader::Create("assets/shaders/FlatColor.glsl");
        m_TextureShader   = Orion::Shader::Create("assets/shaders/Texture.glsl");

        // Textures
        m_Texture     = Orion::Texture2D::Create("assets/textures/Checkerboard.png");
        m_TextureLogo = Orion::Texture2D::Create("assets/textures/fs_logo.png");

        std::dynamic_pointer_cast<Orion::OpenGLShader>(m_TextureShader)->Bind();
        std::dynamic_pointer_cast<Orion::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
    }

    void OnImguiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
        ImGui::End();
    }

    void OnUpdate(Orion::Timestep ts) override
    {
        if (Orion::Input::IsKeyPressed(Orion::Key::Left))
            m_CameraPosition.x -= m_CameraMoveSpeed * ts;
        else if (Orion::Input::IsKeyPressed(Orion::Key::Right))
            m_CameraPosition.x += m_CameraMoveSpeed * ts;

        if (Orion::Input::IsKeyPressed(Orion::Key::Up))
            m_CameraPosition.y += m_CameraMoveSpeed * ts;
        else if (Orion::Input::IsKeyPressed(Orion::Key::Down))
            m_CameraPosition.y -= m_CameraMoveSpeed * ts;

        if (Orion::Input::IsKeyPressed(Orion::Key::Q))
            m_CameraRotation += m_CameraRotationSpeed * ts;
        else if (Orion::Input::IsKeyPressed(Orion::Key::E))
            m_CameraRotation -= m_CameraRotationSpeed * ts;

        Orion::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Orion::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        Orion::Renderer::BeginScene(m_Camera);

        static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        glm::vec4 redColor(0.8f, 0.2f, 0.3f, 1.0f);
        glm::vec4 blueColor(0.2f, 0.3f, 0.8f, 1.0f);

        std::dynamic_pointer_cast<Orion::OpenGLShader>(m_FlatColorShader)->Bind();
        std::dynamic_pointer_cast<Orion::OpenGLShader>(m_FlatColorShader)
            ->UploadUniformFloat3("u_Color", m_SquareColor);

        for (int y = 0; y < 20; y++)
        {
            for (int x = 0; x < 20; x++)
            {
                glm::vec3 pos(static_cast<float>(x) * 0.11f, static_cast<float>(y) * 0.11f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
                Orion::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
            }
        }

        m_Texture->Bind();
        Orion::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));

        m_TextureLogo->Bind();
        Orion::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));

        // Orion::Renderer::Submit(m_Shader, m_VertexArray);

        Orion::Renderer::EndScene();
    }

    void OnEvent(Orion::Event& event) override
    {
        // Orion::EventDispatcher dispatcher(event);
        // dispatcher.Dispatch<Orion::KeyPressedEvent>(OE_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
    }

    bool OnKeyPressedEvent(Orion::KeyPressedEvent& event)
    {
        return false;
    }

private:
    Orion::Ref<Orion::Shader> m_FlatColorShader, m_TextureShader;
    Orion::Ref<Orion::VertexArray> m_SquareVA;

    Orion::Ref<Orion::Texture2D> m_Texture, m_TextureLogo;

    Orion::OrthographicCamera m_Camera;

    glm::vec3 m_CameraPosition;
    float m_CameraRotation = 0.0f;

    float m_CameraMoveSpeed     = 2.0f;
    float m_CameraRotationSpeed = 45.0f;

    glm::vec3 m_SquareColor = {0.2f, 0.3f, 0.8f};
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
