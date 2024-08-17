#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <chrono>


template <typename Ty>
class Timer {
public:
	Timer(const char* name, Ty&& func)
		: m_name(name), m_Stopped(false), m_func(func)
	{
		m_StartPoint = std::chrono::high_resolution_clock::now();
	}

	~Timer() {
		if (!m_Stopped)
			Stop();
	}

	void Stop() {
		auto endTimePoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartPoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		m_Stopped = true;

		auto duration = (end - start) * 0.001f;

		m_func({ m_name,duration });
	}

private:
	const char* m_name;
	bool m_Stopped;
	std::chrono::time_point<std::chrono::steady_clock> m_StartPoint;

	Ty m_func;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileData profileData) {m_Profile.push_back(profileData); })

Sandbox2D::Sandbox2D() 
	:Layer("Render Square and Triangle"), m_orthoCamController(1280.0f / 720.0f) {}

void Sandbox2D:: OnAttach() {
	m_Texture2D = Forge::Texture2D::Create("assets/Textures/CheckerBoard.png");
	m_Texture2D->Bind();
}
void Sandbox2D:: OnDetach() {}
void Sandbox2D:: OnUpdate(Forge::Timestep time) {

	PROFILE_SCOPE("Sandbox OnUpdate");

	
	{
		PROFILE_SCOPE("CameraController OnUpdate");
		m_orthoCamController.OnUpdate(time);
	}

	{
		PROFILE_SCOPE("Renderer Setup");

		Forge::RenderCommands::SetClearColor({ 0.1, 0.1, 0.1, 1 });
		Forge::RenderCommands::Clear();
	}

	{
		PROFILE_SCOPE("Draw calls");
		
		Forge::Renderer2D::BeginScene(m_orthoCamController.GetCamera());

		Forge::Renderer2D::DrawQuad({ 0.5f,0.0f,0.0f }, glm::vec2(1.0f), { m_SquareColor, 1.0f });
		Forge::Renderer2D::DrawQuad({ 1.5f,-0.5f,0.0f }, glm::vec2(0.5f, 0.8f), { 0.8f,0.2f,0.2f, 1.0f });

		Forge::Renderer2D::DrawQuad({ -1.0f,0.5f,-0.1f }, glm::vec2(10.0f), m_Texture2D, 10, { 1.0f,1.0f,1.0f,1.0f });
	}

	Forge::Renderer::EndScene();
}
void Sandbox2D:: OnImGuiRender() {

	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

	for (auto result : m_Profile) {
		char label[50];
		strcpy(label, result.name);
		strcat(label, "  %.3fms");
		ImGui::Text(label, result.time);
	}
	ImGui::End();

	m_Profile.clear();

}
void Sandbox2D:: OnEvent(Forge::Event& e) {
	m_orthoCamController.OnEvent(e);
} 