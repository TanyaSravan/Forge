#pragma once
#include "Core.h"
#include "Window.h"

namespace Forge {
	class FORGE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Window> m_window;
		bool m_Running = true;
	};

	//Definition on Client side
	Application* CreateApplication();
}

