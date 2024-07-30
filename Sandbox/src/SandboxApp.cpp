#include "Forge.h"
#include "Forge/Core/EntryPoint.h"

#include "imgui/imgui.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Sandbox2D.h"


class ExampleLayer : public Forge::Layer {
public:
	ExampleLayer(): Layer("Render Square and Triangle"),
		m_orthoCamController(1280.0f/720.0f)
	{
		m_TriangleVA = Forge::VertexArray::Create();

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

		m_Shader = Forge::Shader::Create("TriangleShader",vertexSrc, fragmentSrc);

		m_SquareVA = Forge::VertexArray::Create();
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

		m_BlueShader = Forge::Shader::Create("Flat Color Shader", bluevertexSrc, bluefragmentSrc);

		Forge::Ref<Forge::Shader> textureShader = m_ShaderLibrary.Load("assets/Shaders/TextureShader.glsl");
		m_Texture2D = Forge::Texture2D::Create("assets/Textures/CheckerBoard.png");
		m_LogoTexture = Forge::Texture2D::Create("assets/Textures/logo.png");

		textureShader->Bind();
		textureShader->SetInt("u_Texture", 0);
	}

	void OnUpdate(Forge::Timestep time) override {

		m_orthoCamController.OnUpdate(time);

		Forge::RenderCommands::SetClearColor({ 0.1, 0.1, 0.1, 1 });
		Forge::RenderCommands::Clear();

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		Forge::Renderer::BeginScene(m_orthoCamController.GetCamera());

		m_BlueShader->Bind();
		m_BlueShader->SetFloat3("u_Color", m_SquareColor);

		for (int x = 0; x < 20; x++) {
			for (int y = 0; y < 20; y++) {
				glm::vec3 pos = glm::vec3(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 squareTransform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Forge::Renderer::Submit(m_BlueShader, m_SquareVA, squareTransform);
			}
		}

		m_Texture2D->Bind();
		glm::mat4 TextureTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
		Forge::Renderer::Submit(m_ShaderLibrary.Get("TextureShader"), m_SquareVA, TextureTransform);
		m_LogoTexture->Bind();
		Forge::Renderer::Submit(m_ShaderLibrary.Get("TextureShader"), m_SquareVA, TextureTransform);
		
		//Forge::Renderer::Submit(m_Shader,m_TriangleVA);
		Forge::Renderer::EndScene();
	}

	void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Forge::Event& e) override {
		m_orthoCamController.OnEvent(e);
	}

	private:
		Forge::ShaderLibrary m_ShaderLibrary;

		Forge::Ref<Forge::Shader> m_Shader;
		Forge::Ref<Forge::VertexArray> m_TriangleVA;

		Forge::Ref<Forge::VertexArray> m_SquareVA;
		Forge::Ref<Forge::Shader> m_BlueShader;
		Forge::Ref<Forge::Texture2D> m_Texture2D, m_LogoTexture;

		glm::vec3 m_SquareColor = { 0.2f,0.6f,0.4f };

		Forge::OrthographicCameraController m_orthoCamController;

};


class Sandbox : public Forge::Application {
public:
	Sandbox() {
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox() {

	}
};

Forge::Application* Forge::CreateApplication() {
	return new Sandbox();
}

