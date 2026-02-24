
#include "ExampleLayer.h"

ExampleLayer::ExampleLayer() : Layer("Example"), m_CameraController(1280.0f / 720.0f, true)
{
    // SQUARE
    m_SquareVA                               = Orion::VertexArray::Create();
    float squareVertices[5 * 4]              = {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
                                                0.5f,  0.5f,  0.0f, 1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f};
    Orion::Ref<Orion::VertexBuffer> squareVB = Orion::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
    Orion::BufferLayout squareVBLayout       = {{Orion::ShaderDataType::Float3, "a_Position"},
                                                {Orion::ShaderDataType::Float2, "a_TexCoord"}};
    squareVB->SetLayout(squareVBLayout);
    m_SquareVA->AddVertexBuffer(squareVB);

    uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
    Orion::Ref<Orion::IndexBuffer> squareIB;
    squareIB = Orion::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
    m_SquareVA->SetIndexBuffer(squareIB);

    // Shaders
    auto m_FlatColorShader = m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");
    auto m_TextureShader   = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

    // Textures
    m_Texture     = Orion::Texture2D::Create("assets/textures/Checkerboard.png");
    m_TextureLogo = Orion::Texture2D::Create("assets/textures/fs_logo.png");

    m_TextureShader->Bind();
    m_TextureShader->SetInt("u_Texture", 0);
}

void ExampleLayer::OnAttach() {}

void ExampleLayer::OnDetach() {}

void ExampleLayer::OnUpdate(Orion::Timestep ts)
{
    // Update
    m_CameraController.OnUpdate(ts);

    // Render
    Orion::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    Orion::RenderCommand::Clear();

    Orion::Renderer::BeginScene(m_CameraController.GetCamera());

    static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

    auto m_FlatColorShader = m_ShaderLibrary.Get("FlatColor");

    m_FlatColorShader->Bind();
    m_FlatColorShader->SetFloat3("u_Color", m_SquareColor);

    for (int y = 0; y < 20; y++)
    {
        for (int x = 0; x < 20; x++)
        {
            glm::vec3 pos(static_cast<float>(x) * 0.11f, static_cast<float>(y) * 0.11f, 0.0f);
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
            Orion::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
        }
    }

    auto m_TextureShader = m_ShaderLibrary.Get("Texture");
    m_Texture->Bind();
    Orion::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));

    m_TextureLogo->Bind();
    Orion::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));

    Orion::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void ExampleLayer::OnEvent(Orion::Event& e)
{
    m_CameraController.OnEvent(e);
}
