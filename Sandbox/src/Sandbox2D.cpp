#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Sandbox2D::Sandbox2D() 
	:Layer("Render Square and Triangle"), m_orthoCamController(1280.0f / 720.0f) {}

void Sandbox2D:: OnAttach() {
	m_Texture2D = Forge::Texture2D::Create("assets/Textures/CheckerBoard.png");
	m_Texture2D->Bind();
}
void Sandbox2D:: OnDetach() {}
void Sandbox2D:: OnUpdate(Forge::Timestep time) {

	m_orthoCamController.OnUpdate(time);

	Forge::RenderCommands::SetClearColor({ 0.1, 0.1, 0.1, 1 });
	Forge::RenderCommands::Clear();

	Forge::Renderer2D::BeginScene(m_orthoCamController.GetCamera());

	Forge::Renderer2D::DrawQuad({ 0.5f,0.0f,0.0f }, glm::vec2(1.0f), { m_SquareColor, 1.0f });
	Forge::Renderer2D::DrawQuad({ 1.5f,-0.5f,0.0f }, glm::vec2(0.5f, 0.8f), { 0.8f,0.2f,0.2f, 1.0f });

	Forge::Renderer2D::DrawQuad({ -1.0f,0.5f,-0.1f }, glm::vec2(10.0f), m_Texture2D);

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