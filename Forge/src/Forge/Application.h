#pragma once
#include "Core.h"
#include "Window.h"
#include "Forge/Event/Event.h"
#include "Forge/Event/ApplicationEvents.h"

namespace Forge {
	class FORGE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_window;
		bool m_Running = true;
	};

	//Definition on Client side
	Application* CreateApplication();
}

