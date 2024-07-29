#include "Sandbox2D.h"
#include "Platform/OpenGL/OpenGlShader.h"

#include "imgui/imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Sandbox2D::Sandbox2D() 
	:Layer("Render Square and Triangle"), m_orthoCamController(1280.0f / 720.0f) {}

void Sandbox2D:: OnAttach() {

	m_SquareVA = Forge::VertexArray::Create();
	float Squarevertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
	};

	Forge::Ref<Forge::VertexBuffer> squareVB;
	squareVB = Forge::VertexBuffer::Create(Squarevertices, sizeof(Squarevertices));

	Forge::BufferLayout Squarelayout = {
		{Forge::ShaderDataType::Float3, "position"},
		{Forge::ShaderDataType::Float2, "TexCoords"},
	};

	squareVB->SetLayout(Squarelayout);
	m_SquareVA->AddVertexBuffer(squareVB);

	unsigned int Squareindices[6] = { 0, 1, 2, 2, 3, 0 };
	Forge::Ref<Forge::IndexBuffer>squareIB;
	squareIB = Forge::IndexBuffer::Create(Squareindices, 6);

	m_SquareVA->SetIndexBuffer(squareIB);

	m_BlueShader = Forge::Shader::Create("assets/Shaders/FlatColor.glsl");

}
void Sandbox2D:: OnDetach() {}
void Sandbox2D:: OnUpdate(Forge::Timestep time) {

	m_orthoCamController.OnUpdate(time);

	Forge::RenderCommands::SetClearColor({ 0.1, 0.1, 0.1, 1 });
	Forge::RenderCommands::Clear();

	Forge::Renderer::BeginScene(m_orthoCamController.GetCamera());

	std::dynamic_pointer_cast<Forge::OpenGLShader>(m_BlueShader)->Bind();
	std::dynamic_pointer_cast<Forge::OpenGLShader>(m_BlueShader)->UploadUniformFloat3("u_Color", m_SquareColor);

	Forge::Renderer::Submit(m_BlueShader, m_SquareVA);

	Forge::Renderer::EndScene();
}
void Sandbox2D:: OnImGuiRender() {

	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();

}
void Sandbox2D:: OnEvent(Forge::Event& e) {
	m_orthoCamController.OnEvent(e);
}