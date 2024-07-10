#include "FGpch.h"
#include "Application.h"
#include "GLFW/glfw3.h"

namespace Forge {

#define FG_BIND_EVENT_FN(x) std::bind(&Application::x,this,std::placeholders::_1)

	Application::Application(){
		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(FG_BIND_EVENT_FN(OnEvent));
	}

	Application::~Application(){}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(FG_BIND_EVENT_FN(OnWindowClose));

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
	}

	void Application::PushOverlay(Layer* layer) {
		m_layerStack.PushOverlay(layer);
	}


	void Application::Run() {
		while (m_Running) {
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_layerStack) {
				layer->OnUpdate();
			}

			m_window->OnUpdate();
		}
	}
}



