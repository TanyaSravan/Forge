#include "Forge.h"
#include "imgui/imgui.h"


class ExampleLayer : public Forge::Layer {
public:
	ExampleLayer(): Layer("Render Square and Triangle"),
		m_orthoCam({ -1.6, 1.6, -0.9, 0.9 })
	{
		m_TriangleVA.reset(Forge::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,	0.8f, 0.2f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f,	0.2f, 0.3f, 0.8f, 1.0f
		};

		std::shared_ptr<Forge::VertexBuffer> triangleVB;
		triangleVB.reset(Forge::VertexBuffer::Create(vertices, sizeof(vertices)));


		Forge::BufferLayout layout = {
			{Forge::ShaderDataType::Float3, "position"},
			{Forge::ShaderDataType::Float4, "color"},
		};

		triangleVB->SetLayout(layout);

		m_TriangleVA->AddVertexBuffer(triangleVB);

		unsigned int indices[3] = { 0, 1, 2 };

		std::shared_ptr<Forge::IndexBuffer> triangleIB;
		triangleIB.reset(Forge::IndexBuffer::Create(indices, 3));
		m_TriangleVA->SetIndexBuffer(triangleIB);

		std::string vertexSrc = R"(
			#version 410 core

			layout(location = 0) in vec4 position;
			layout(location = 1) in vec4 color;
			
			uniform mat4 u_VP;

			out vec4 v_position;
			out vec4 v_color;

			void main(){
				gl_Position = u_VP * position;
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

		m_Shader.reset(Forge::Shader::Create(vertexSrc, fragmentSrc));

		m_SquareVA.reset(Forge::VertexArray::Create());
		float Squarevertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f,
		};

		std::shared_ptr<Forge::VertexBuffer> squareVB;
		squareVB.reset(Forge::VertexBuffer::Create(Squarevertices, sizeof(Squarevertices)));

		Forge::BufferLayout Squarelayout = {
			{Forge::ShaderDataType::Float3, "position"},
		};

		squareVB->SetLayout(Squarelayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int Squareindices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Forge::IndexBuffer>squareIB;
		squareIB.reset(Forge::IndexBuffer::Create(Squareindices, 6));

		m_SquareVA->SetIndexBuffer(squareIB);

		std::string bluevertexSrc = R"(
			#version 410 core
			layout(location = 0) in vec4 position;

			uniform mat4 u_VP;

			void main(){
				gl_Position = u_VP * position;
			};
		)";

		std::string bluefragmentSrc = R"(
			#version 410 core

			layout(location = 0) out vec4 color;

			void main(){
			   color = vec4(0.2f,0.6f,0.6f,1.0f);
			};
		)";

		m_BlueShader.reset(Forge::Shader::Create(bluevertexSrc, bluefragmentSrc));
	}

	void OnUpdate() override {
		Forge::RenderCommands::SetClearColor({ 0.1, 0.1, 0.1, 1 });
		Forge::RenderCommands::Clear();

		m_orthoCam.SetPosition({ 0.5f,0.0f,0.0f });
		m_orthoCam.SetRotation(45.0f);

		Forge::Renderer::BeginScene(m_orthoCam);
		Forge::Renderer::Submit(m_BlueShader,m_SquareVA);
		Forge::Renderer::Submit(m_Shader,m_TriangleVA);

		Forge::Renderer::EndScene();
	}

	void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(Forge::Event& event) override {
		if (event.GetEventType() == Forge::EventType::KeyPressed) {
			Forge::KeyPressedEvent& e = (Forge::KeyPressedEvent&)event;
			if (e.GetKeyCode() == FG_KEY_TAB)
				FG_TRACE("Tab Key Was pressed (Event)"); 
			FG_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

	private:
		std::shared_ptr<Forge::Shader> m_Shader;
		std::shared_ptr<Forge::VertexArray> m_TriangleVA;

		std::shared_ptr<Forge::VertexArray> m_SquareVA;
		std::shared_ptr<Forge::Shader> m_BlueShader;

		Forge::OrthographicCamera m_orthoCam;
};


class Sandbox : public Forge::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Forge::Application* Forge::CreateApplication() {
	return new Sandbox();
}

