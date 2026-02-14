#include <Orion.h>

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Orion::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
    {
        /// TRIANGLE
        m_VertexArray.reset(Orion::VertexArray::Create());

        float vertices[3 * 7] = {-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.5f, -0.5f, 0.0f, 0.0f,
                                 0.0f,  1.0f,  1.0f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  1.0f};
        std::shared_ptr<Orion::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Orion::VertexBuffer::Create(vertices, sizeof(vertices)));
        Orion::BufferLayout layout = {{Orion::ShaderDataType::Float3, "a_Position"},
                                      {Orion::ShaderDataType::Float4, "a_Color"}};
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        uint32_t indices[3] = {0, 1, 2};
        std::shared_ptr<Orion::IndexBuffer> indexBuffer;
        indexBuffer.reset(Orion::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        /// SQUARE
        m_SquareVA.reset(Orion::VertexArray::Create());
        float squareVertices[3 * 4] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f};
        std::shared_ptr<Orion::VertexBuffer> squareVB;
        squareVB.reset(Orion::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        Orion::BufferLayout squareVBLayout = {{Orion::ShaderDataType::Float3, "a_Position"}};
        squareVB->SetLayout(squareVBLayout);
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<Orion::IndexBuffer> squareIB;
        squareIB.reset(Orion::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);

        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;
            out vec4 v_Color;

            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
            }
        )";

        m_Shader.reset(new Orion::Shader(vertexSrc, fragmentSrc));

        std::string blueShaderVertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;

            void main()
            {
                v_Position = a_Position;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

        std::string blueShaderFragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;

            void main()
            {
                color = vec4(0.2, 0.3, 0.8, 1.0);
            }
        )";

        m_BlueShader.reset(new Orion::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
    }

    void OnImguiRender() override {}

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

        for (int y = 0; y < 5; y++)
            for (int x = 0; x < 5; x++)
            {
                glm::vec3 pos(static_cast<float>(x) * 0.11f, static_cast<float>(y) * 0.11f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
                Orion::Renderer::Submit(m_BlueShader, m_SquareVA, transform);
            }

        Orion::Renderer::Submit(m_Shader, m_VertexArray);

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
    std::shared_ptr<Orion::Shader> m_Shader;
    std::shared_ptr<Orion::VertexArray> m_VertexArray;

    std::shared_ptr<Orion::Shader> m_BlueShader;
    std::shared_ptr<Orion::VertexArray> m_SquareVA;

    Orion::OrthographicCamera m_Camera;

    glm::vec3 m_CameraPosition;
    float m_CameraRotation = 0.0f;

    float m_CameraMoveSpeed     = 2.0f;
    float m_CameraRotationSpeed = 45.0f;
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
