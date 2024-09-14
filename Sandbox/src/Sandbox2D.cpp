#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


Sandbox2D::Sandbox2D() 
	:Layer("Render Square and Triangle"), m_orthoCamController(1280.0f / 720.0f) {}

void Sandbox2D:: OnAttach() {
	FG_PROFILE_FUNCTION();
	m_Texture2D = Forge::Texture2D::Create("assets/Textures/CheckerBoard.png");

}
void Sandbox2D:: OnDetach() {
	FG_PROFILE_FUNCTION();
}

static float rot = 0.0f;
void Sandbox2D:: OnUpdate(Forge::Timestep time) {

	FG_PROFILE_FUNCTION();
	rot += 45 * time;

	m_orthoCamController.OnUpdate(time);

	{
		FG_PROFILE_SCOPE("Renderer Setup");

		Forge::RenderCommands::SetClearColor({ 0.1, 0.1, 0.1, 1 });
		Forge::RenderCommands::Clear();
	}

	Forge::Renderer2D::ResetStats();


	Forge::Renderer2D::BeginScene(m_orthoCamController.GetCamera());

	Forge::Renderer2D::DrawQuad({ 0.5f,0.0f,0.0f }, glm::vec2(1.0f), { m_SquareColor, 1.0f });
	Forge::Renderer2D::DrawRotatedQuad(glm::vec3(5.0f,0.0f,0.0f), glm::vec2(1.0f), 45.0f, { m_SquareColor, 1.0f });
	Forge::Renderer2D::DrawQuad({ 1.5f,-0.5f,0.0f }, glm::vec2(0.5f, 0.8f), { 0.8f,0.2f,0.2f, 1.0f });
	Forge::Renderer2D::DrawQuad({ -5.0f,-5.0f,-0.1f }, glm::vec2(10.0f,10.0f), m_Texture2D, 10);
	Forge::Renderer2D::DrawRotatedQuad({ 0.0f,0.0f,0.0f }, glm::vec2(1.0f,1.0f),rot, m_Texture2D,{0.2,0.8,0.2,1.0f});

	Forge::Renderer2D::EndScene();

	Forge::Renderer2D::BeginScene(m_orthoCamController.GetCamera());

	for (float y = -5.0f; y < 5.0f; y += 0.1f) {
		for (float x = -5.0f; x < 5.0f; x += 0.1f) {
			glm::vec4 color = { (x + 5.0f) / 10, 0.3, (y + 5.0f )/ 10,0.5f };
			Forge::Renderer2D::DrawQuad({ x,y,0.0f }, glm::vec2(0.45f, 0.45f), color);
		}
	}

	Forge::Renderer2D::EndScene();

}
void Sandbox2D:: OnImGuiRender() {

	FG_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::Text("RendererStats: ");
	ImGui::Text("    Draw Calls: %d", Forge::Renderer2D::GetStat().DrawCalls);
	ImGui::Text("    NumQuads: %d", Forge::Renderer2D::GetStat().NumQuads);
	ImGui::End();

}
void Sandbox2D:: OnEvent(Forge::Event& e) {
	m_orthoCamController.OnEvent(e);
} 