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
		FG_CORE_TRACE("{0}", e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(FG_BIND_EVENT_FN(OnWindowClose));
	}


	void Application::Run() {
		while (m_Running) {
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}



