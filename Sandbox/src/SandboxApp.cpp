#include "Forge.h"

#include "Platform/OpenGL/OpenGlShader.h"

#include "imgui/imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


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

		Forge::Ref<Forge::VertexBuffer> triangleVB;
		triangleVB = Forge::VertexBuffer::Create(vertices, sizeof(vertices));


		Forge::BufferLayout layout = {
			{Forge::ShaderDataType::Float3, "position"},
			{Forge::ShaderDataType::Float4, "color"},
		};

		triangleVB->SetLayout(layout);

		m_TriangleVA->AddVertexBuffer(triangleVB);

		unsigned int indices[3] = { 0, 1, 2 };

		Forge::Ref<Forge::IndexBuffer> triangleIB;
		triangleIB = Forge::IndexBuffer::Create(indices, 3);
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

		m_Shader = Forge::Shader::Create(vertexSrc, fragmentSrc);

		m_SquareVA.reset(Forge::VertexArray::Create());
		float Squarevertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		};

		Forge::Ref<Forge::VertexBuffer> squareVB;
		squareVB = Forge::VertexBuffer::Create(Squarevertices, sizeof(Squarevertices));

		Forge::BufferLayout Squarelayout = {
			{Forge::ShaderDataType::Float3, "position"},
			{Forge::ShaderDataType::Float2, "TexCoords"},
		};

		squareVB->SetLayout(Squarelayout);
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int Squareindices[6] = { 0, 1, 2, 2, 3, 0 };
		Forge::Ref<Forge::IndexBuffer>squareIB;
		squareIB = Forge::IndexBuffer::Create(Squareindices, 6);

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
			uniform vec3 u_Color;

			void main(){
			   color = vec4(u_Color,1.0f);
			};
		)";

		m_BlueShader = Forge::Shader::Create(bluevertexSrc, bluefragmentSrc);

		std::string TexvertexSrc = R"(
			#version 410 core
			layout(location = 0) in vec4 position;
			layout(location = 1) in vec2 TexCoord;

			uniform mat4 u_VP;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main(){
				v_TexCoord = TexCoord;
				gl_Position = u_VP * u_Transform * position;
			};
		)";

		std::string TexfragmentSrc = R"(
			#version 410 core

			layout(location = 0) out vec4 color;
				
			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main(){
			   color = texture(u_Texture,v_TexCoord);
			};
		)";

		m_TextureShader = Forge::Shader::Create(TexvertexSrc, TexfragmentSrc);
		m_Texture2D = Forge::Texture2D::Create("assets/logo.png");

		std::dynamic_pointer_cast<Forge::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Forge::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Forge::Timestep time) override {

		if (Forge::Input::IsKeyPressed(FG_KEY_A))
			m_CamPosition.x += m_CamMoveSpeed * time;
		else if (Forge::Input::IsKeyPressed(FG_KEY_D))
			m_CamPosition.x -= m_CamMoveSpeed * time;
		if (Forge::Input::IsKeyPressed(FG_KEY_W))
			m_CamPosition.y += m_CamMoveSpeed * time;
		else if (Forge::Input::IsKeyPressed(FG_KEY_S))
			m_CamPosition.y -= m_CamMoveSpeed * time;

		if (Forge::Input::IsKeyPressed(FG_KEY_RIGHT))
			m_CamRotation += m_CamRotSpeed * time;
		else if (Forge::Input::IsKeyPressed(FG_KEY_LEFT))
			m_CamRotation -= m_CamRotSpeed * time;


		Forge::RenderCommands::SetClearColor({ 0.1, 0.1, 0.1, 1 });
		Forge::RenderCommands::Clear();

		m_orthoCam.SetPosition(m_CamPosition);
		m_orthoCam.SetRotation(m_CamRotation);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		Forge::Renderer::BeginScene(m_orthoCam);

		std::dynamic_pointer_cast<Forge::OpenGLShader>(m_BlueShader)->Bind();
		std::dynamic_pointer_cast<Forge::OpenGLShader>(m_BlueShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int x = 0; x < 20; x++) {
			for (int y = 0; y < 20; y++) {
				glm::vec3 pos = glm::vec3(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 squareTransform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Forge::Renderer::Submit(m_BlueShader, m_SquareVA, squareTransform);
			}
		}

		m_Texture2D->Bind();
		glm::mat4 TextureTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
		Forge::Renderer::Submit(m_TextureShader, m_SquareVA, TextureTransform);
		
		//Forge::Renderer::Submit(m_Shader,m_TriangleVA);
		Forge::Renderer::EndScene();
	}

	void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
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
		Forge::Ref<Forge::Shader> m_Shader;
		Forge::Ref<Forge::VertexArray> m_TriangleVA;

		Forge::Ref<Forge::VertexArray> m_SquareVA;
		Forge::Ref<Forge::Shader> m_BlueShader, m_TextureShader;
		Forge::Ref<Forge::Texture2D> m_Texture2D;

		glm::vec3 m_SquareColor = { 0.2f,0.6f,0.4f };

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

