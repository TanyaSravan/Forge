#pragma once
#include "Core.h"
#include "Window.h"
#include "Forge/Event/Event.h"
#include "Forge/Event/ApplicationEvents.h"
#include "LayerStack.h"

namespace Forge {
	class FORGE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);

		void PushOverlay(Layer* layer);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_window;
		bool m_Running = true;
		LayerStack m_layerStack;
	};

	//Definition on Client side
	Application* CreateApplication();
}

