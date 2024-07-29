 #include "FGpch.h"
#include "Application.h"
#include "Input.h"
#include "Forge/ImGui/ImguiLayer.h"
#include "Forge/Renderer/Renderer.h"
#include "Forge/Core/Timestep.h"

#include "GLFW/glfw3.h"

namespace Forge {

	Application* Application::s_Instance = nullptr;

	Application::Application(){
		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(FG_BIND_EVENT_FN(Application::OnEvent));

		FG_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		Renderer::Init();

		imgui_layer = new ImGuiLayer();
		PushOverlay(imgui_layer);
	}

	Application::~Application(){

	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(FG_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(FG_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer) {
		m_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		m_layerStack.PushOverlay(layer);
		layer->OnAttach();
	}


	void Application::Run() {
		while (m_Running) {

			float time = (float)glfwGetTime();
			Timestep deltatime = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimised) {
				for (Layer* layer : m_layerStack) {
					layer->OnUpdate(deltatime);
				}
			}


			imgui_layer->Begin();

			for (Layer* layer : m_layerStack) {
				layer->OnImGuiRender();
			}

			imgui_layer->End();

			m_window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {

		if (e.GetWindowWidth() == 0 || e.GetWindowHeight() == 0) {
			m_Minimised = true;
		}

		m_Minimised = false;

		Renderer::SetViewport(e.GetWindowWidth(), e.GetWindowHeight());

		return false;
	}
}



