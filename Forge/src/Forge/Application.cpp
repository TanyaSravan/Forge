 #include "FGpch.h"
#include "Application.h"
#include "Input.h"
#include "Forge/ImGui/ImguiLayer.h"
#include "Forge/Renderer/Renderer.h"

namespace Forge {

	Application* Application::s_Instance = nullptr;

	Application::Application(){
		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(FG_BIND_EVENT_FN(Application::OnEvent));

		FG_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		imgui_layer = new ImGuiLayer();
		PushOverlay(imgui_layer);
	}

	Application::~Application(){

	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(FG_BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
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

			for (Layer* layer : m_layerStack) {
				layer->OnUpdate();
			}

			imgui_layer->Begin();

			for (Layer* layer : m_layerStack) {
				layer->OnImGuiRender();
			}
			imgui_layer->End();

			m_window->OnUpdate();
		}
	}
}



