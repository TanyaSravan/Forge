 #include "FGpch.h"
#include "Application.h"
#include "Input.h"
#include "glad/glad.h"
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
		
		m_TriangleVA.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,	0.8f, 0.2f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f,	0.2f, 0.3f, 0.8f, 1.0f
		};

		std::shared_ptr<VertexBuffer> triangleVB;
		triangleVB.reset(VertexBuffer::Create(vertices, sizeof(vertices)));


		BufferLayout layout = {
			{ShaderDataType::Float3, "position"},
			{ShaderDataType::Float4, "color"},
		};

		triangleVB->SetLayout(layout);

		m_TriangleVA->AddVertexBuffer(triangleVB);

		unsigned int indices[3] = { 0, 1, 2 };

		std::shared_ptr<IndexBuffer> triangleIB;
		triangleIB.reset(IndexBuffer::Create(indices, 3));
		m_TriangleVA->SetIndexBuffer(triangleIB);

		std::string vertexSrc = R"(
			#version 410 core

			layout(location = 0) in vec4 position;
			layout(location = 1) in vec4 color;

			out vec4 v_position;
			out vec4 v_color;

			void main(){
				gl_Position = position;
				v_color = color;
				v_position = position;
			};
		)";

		std::string fragmentSrc = R"(
			#version 410 core

			layout(location = 0) out vec4 color;
			in vec4 v_position;
			in vec4 v_color;

			void main(){
			   color = v_position * 0.5 + 0.5;
			   color = v_color;
			};
		)";

		m_Shader.reset(Shader::Create(vertexSrc, fragmentSrc));

		m_SquareVA.reset(VertexArray::Create());
		float Squarevertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,	
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f,
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(Squarevertices, sizeof(Squarevertices)));

		BufferLayout Squarelayout = {
			{ShaderDataType::Float3, "position"},
		};

		squareVB->SetLayout(Squarelayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int Squareindices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer>squareIB;
		squareIB.reset(IndexBuffer::Create(Squareindices, 6));

		m_SquareVA->SetIndexBuffer(squareIB);

		std::string bluevertexSrc = R"(
			#version 410 core
			layout(location = 0) in vec4 position;

			void main(){
				gl_Position = position;
			};
		)";

		std::string bluefragmentSrc = R"(
			#version 410 core

			layout(location = 0) out vec4 color;

			void main(){
			   color = vec4(0.2f,0.6f,0.6f,1.0f);
			};
		)";

		m_BlueShader.reset(Shader::Create(bluevertexSrc, bluefragmentSrc));

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

			RenderCommands::SetClearColor({ 0.1, 0.1, 0.1, 1 });
			RenderCommands::Clear();

			Renderer::BeginScene();

			m_BlueShader->Bind();
			RenderCommands::DrawIndexed(m_SquareVA);

			m_Shader->Bind();
			RenderCommands::DrawIndexed(m_TriangleVA);

			Renderer::EndScene();

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



