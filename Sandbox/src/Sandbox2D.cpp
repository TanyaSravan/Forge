#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


Sandbox2D::Sandbox2D() 
	:Layer("Render Square and Triangle"), m_orthoCamController(1280.0f / 720.0f) {}

void Sandbox2D:: OnAttach() {
	FG_PROFILE_FUNCTION();
	m_Texture2D = Forge::Texture2D::Create("assets/Textures/CheckerBoard.png");
	m_SpriteSheet = Forge::Texture2D::Create("assets/Textures/tilemap_packed.png");
	m_Bench = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 0,0 }, { 32,32 }, { 1,3 });
	m_Rocks = Forge::SubTextures2D::CreateSubTexture(m_SpriteSheet, { 4,6 }, { 32,32 });

	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.Lifetime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };

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

#if 0
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
#endif


	if (Forge::Input::IsMouseButtonPressed(FG_MOUSE_BUTTON_1))
	{
		auto [x, y] = Forge::Input::GetMousePosition();
		auto width = Forge::Application::Get().GetWindow().GetWidth();
		auto height = Forge::Application::Get().GetWindow().GetHeight();

		auto bounds = m_orthoCamController.GetBounds();
		auto pos = m_orthoCamController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	m_ParticleSystem.OnUpdate(time);
	m_ParticleSystem.OnRender(m_orthoCamController.GetCamera());


	Forge::Renderer2D::BeginScene(m_orthoCamController.GetCamera());
	Forge::Renderer2D::DrawQuad({ 0.0f,0.0f,0.5f }, glm::vec2(1.0f, 3.0f), m_Bench);
	Forge::Renderer2D::DrawQuad({ 1.0f,0.0f,0.5f }, glm::vec2(1.0f, 1.0f), m_Rocks);
	Forge::Renderer2D::EndScene();
	
}
void Sandbox2D:: OnImGuiRender() {

	FG_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	//ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::ColorEdit3("Begin Color", glm::value_ptr(m_Particle.ColorBegin));
	ImGui::ColorEdit3("End Color", glm::value_ptr(m_Particle.ColorEnd));
	ImGui::Text("RendererStats: ");
	ImGui::Text("    Draw Calls: %d", Forge::Renderer2D::GetStat().DrawCalls);
	ImGui::Text("    NumQuads: %d", Forge::Renderer2D::GetStat().NumQuads);
	ImGui::Text("    Num Vertices: %d", Forge::Renderer2D::GetStat().GetNumVertices());
	ImGui::Text("    Num Indices: %d", Forge::Renderer2D::GetStat().GetNumIndices());
	ImGui::End();

}
void Sandbox2D:: OnEvent(Forge::Event& e) {
	m_orthoCamController.OnEvent(e);
} 