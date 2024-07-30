#include "Sandbox2D.h"
#include "Platform/OpenGL/OpenGlShader.h"

#include "imgui/imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Sandbox2D::Sandbox2D() 
	:Layer("Render Square and Triangle"), m_orthoCamController(1280.0f / 720.0f) {}

void Sandbox2D:: OnAttach() {

}
void Sandbox2D:: OnDetach() {}
void Sandbox2D:: OnUpdate(Forge::Timestep time) {

	m_orthoCamController.OnUpdate(time);

	Forge::RenderCommands::SetClearColor({ 0.1, 0.1, 0.1, 1 });
	Forge::RenderCommands::Clear();

	Forge::Renderer2D::BeginScene(m_orthoCamController.GetCamera());

	Forge::Renderer2D::DrawQuad({ 0.0f,0.0f,0.0f }, glm::vec2(1.0f), { m_SquareColor, 0.0f });

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