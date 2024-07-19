 #include "FGpch.h"
#include "Application.h"
#include "Input.h"
#include "glad/glad.h"
#include "Forge/ImGui/ImguiLayer.h"

namespace Forge {

	Application* Application::s_Instance = nullptr;

	static GLenum GetShaderDataTypetoOpenGLBaseType(const ShaderDataType& type) {
		switch (type)
		{
		case Forge::ShaderDataType::None:			FG_CORE_ASSERT(false, "Does notsupport none ShaderType"); return GL_NONE;
		case Forge::ShaderDataType::Float:			return GL_FLOAT;
		case Forge::ShaderDataType::Float2:			return GL_FLOAT;
		case Forge::ShaderDataType::Float3:			return GL_FLOAT;
		case Forge::ShaderDataType::Float4:			return GL_FLOAT;
		case Forge::ShaderDataType::Int:			return GL_INT;
		case Forge::ShaderDataType::Int2:			return GL_INT;
		case Forge::ShaderDataType::Int3:			return GL_INT;
		case Forge::ShaderDataType::Int4:			return GL_INT;
		case Forge::ShaderDataType::Mat3:			return GL_FLOAT;
		case Forge::ShaderDataType::Mat4:			return GL_FLOAT;
		case Forge::ShaderDataType::Bool:			return GL_BOOL;
		}

		FG_CORE_ASSERT(false, "Unknown Shader DataType");
		return 0;
	}

	Application::Application(){
		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(FG_BIND_EVENT_FN(Application::OnEvent));

		FG_CORE_ASSERT(!s_Instance, "Application already exists");
		s_Instance = this;

		imgui_layer = new ImGuiLayer();
		PushOverlay(imgui_layer);
		
		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,	0.2f, 0.8f, 0.3f, 1.0f,
			 0.0f,  0.5f, 0.0f,	0.8f, 0.8f, 0.2f, 1.0f
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
				{ShaderDataType::Float3, "position"},
				{ShaderDataType::Float4, "color"},
			};

			m_VertexBuffer->SetLayout(layout);
		}

		const auto& layout = m_VertexBuffer->GetLayout();
		unsigned int index = 0;

		for (auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetElementCount(element.Type),
				GetShaderDataTypetoOpenGLBaseType(element.Type),
				element.Normalised ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}

		unsigned int indices[3] = { 0, 1, 2 };

		m_IndexBuffer.reset(IndexBuffer::Create(indices, 3));

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
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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



