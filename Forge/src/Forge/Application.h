#pragma once
#include "Core.h"
#include "Window.h"
#include "Forge/Event/Event.h"
#include "Forge/Event/ApplicationEvents.h"
#include "LayerStack.h"
#include "Forge/ImGui/ImguiLayer.h"
#include "Renderer/Shader.h"
#include "Renderer/Buffers.h"
#include "Renderer/VertexArray.h"

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
		ImGuiLayer* imgui_layer;
		bool m_Running = true;
		LayerStack m_layerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_TriangleVA;

		std::shared_ptr<VertexArray> m_SquareVA;
		std::shared_ptr<Shader> m_BlueShader;
	private:
		static Application* s_Instance;
	};

	//Definition on Client side
	Application* CreateApplication();
}

