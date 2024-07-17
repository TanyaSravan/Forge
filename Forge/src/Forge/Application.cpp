 #include "FGpch.h"
#include "Application.h"
#include "Input.h"
#include "glad/glad.h"
#include "Forge/ImGui/ImguiLayer.h"



namespace Forge {

	Application* Application::s_Instance = nullptr;

	Application::Application(){
		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(FG_BIND_EVENT_FN(Application::OnEvent));

		FG_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		imgui_layer = new ImGuiLayer();
		PushOverlay(imgui_layer);
		
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		unsigned int indexBuffer[3] = { 0, 1, 2 };

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexBuffer), &indexBuffer, GL_STATIC_DRAW);

		std::string vertexSrc = R"(
			#version 410 core

			layout(location = 0) in vec4 position;

			out vec4 v_position;

			void main(){
				gl_Position = position;
				v_position = position;
			};
		)";

		std::string fragmentSrc = R"(
			#version 410 core

			layout(location = 0) out vec4 color;
			in vec4 v_position;

			void main(){
			   color = v_position * 0.5 + 0.5;
			};
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
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
			glClearColor(0.1, 0.1, 0.1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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



