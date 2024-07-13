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

		inline Window& GetWindow() { return *m_window; }

		static inline Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_window;
		bool m_Running = true;
		LayerStack m_layerStack;
	private:
		static Application* s_Instance;
	};

	//Definition on Client side
	Application* CreateApplication();
}

