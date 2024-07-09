#include "FGpch.h"
#include "Application.h"
#include "Forge/Event/Event.h"
#include "Forge/Event/ApplicationEvents.h"
#include "GLFW/glfw3.h"

namespace Forge {

	Application::Application(){
		m_window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application(){}


	void Application::Run() {

		while (m_Running) {
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_window->OnUpdate();
		}
	}
}



