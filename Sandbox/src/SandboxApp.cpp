#include "Forge.h"
#include "imgui/imgui.h"
#include "glm/gtc/matrix_transform.hpp"


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
			uniform mat4 u_Transform;

			out vec4 v_position;
			out vec4 v_color;

			void main(){
				gl_Position = u_VP * u_Transform * position;
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
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
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
			uniform mat4 u_Transform;

			void main(){
				gl_Position = u_VP * u_Transform * position;
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

	void OnUpdate(Forge::Timestep time) override {

		if (Forge::Input::IsKeyPressed(FG_KEY_LEFT))
			m_CamPosition.x += m_CamMoveSpeed * time;
		else if (Forge::Input::IsKeyPressed(FG_KEY_RIGHT))
			m_CamPosition.x -= m_CamMoveSpeed * time;
		if (Forge::Input::IsKeyPressed(FG_KEY_UP))
			m_CamPosition.y += m_CamMoveSpeed * time;
		else if (Forge::Input::IsKeyPressed(FG_KEY_DOWN))
			m_CamPosition.y -= m_CamMoveSpeed * time;

		if (Forge::Input::IsKeyPressed(FG_KEY_A))
			m_CamRotation += m_CamRotSpeed * time;
		else if (Forge::Input::IsKeyPressed(FG_KEY_D))
			m_CamRotation -= m_CamRotSpeed * time;


		Forge::RenderCommands::SetClearColor({ 0.1, 0.1, 0.1, 1 });
		Forge::RenderCommands::Clear();

		m_orthoCam.SetPosition(m_CamPosition);
		m_orthoCam.SetRotation(m_CamRotation);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		Forge::Renderer::BeginScene(m_orthoCam);

		for (int x = 0; x < 20; x++) {
			for (int y = 0; y < 20; y++) {
				glm::vec3 pos = glm::vec3(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 squareTransform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Forge::Renderer::Submit(m_BlueShader, m_SquareVA, squareTransform);
			}
		}

		Forge::Renderer::Submit(m_Shader,m_TriangleVA);
		Forge::Renderer::EndScene();
	}

	void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(Forge::Event& event) override {
		Forge::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Forge::KeyPressedEvent>(FG_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Forge::KeyPressedEvent& event) {

		return false;
	}

	private:
		std::shared_ptr<Forge::Shader> m_Shader;
		std::shared_ptr<Forge::VertexArray> m_TriangleVA;

		std::shared_ptr<Forge::VertexArray> m_SquareVA;
		std::shared_ptr<Forge::Shader> m_BlueShader;

		Forge::OrthographicCamera m_orthoCam;
		glm::vec3 m_CamPosition = { 0.0f,0.0f,0.0f };
		float m_CamRotation = 0.0f;
		float m_CamMoveSpeed = 5.0f;
		float m_CamRotSpeed = 180.0f;
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

