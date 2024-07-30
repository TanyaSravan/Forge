#include "FGpch.h"
#include "Renderer2D.h"
#include "RenderCommands.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGlShader.h"

namespace Forge {

	struct Renderer2DStorage {
		Ref<VertexArray> SquareVA;
		Ref<Shader> BlueShader;
	};

	static Renderer2DStorage* s_data;

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_data->BlueShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_data->BlueShader)->UploadUniformMat4("u_VP", camera.GetVPMatrix());
	}
	void Renderer2D::Init()
	{
		s_data = new Renderer2DStorage();
		s_data->SquareVA = VertexArray::Create();
		float Squarevertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
		};

		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(Squarevertices, sizeof(Squarevertices));

		BufferLayout Squarelayout = {
			{ShaderDataType::Float3, "position"},
		};

		squareVB->SetLayout(Squarelayout);
		s_data->SquareVA->AddVertexBuffer(squareVB);

		unsigned int Squareindices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer>squareIB;
		squareIB = IndexBuffer::Create(Squareindices, 6);

		s_data->SquareVA->SetIndexBuffer(squareIB);

		s_data->BlueShader = Shader::Create("assets/Shaders/FlatColor.glsl");
	}
	void Renderer2D::Shutdown()
	{
		delete s_data;
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2 size, const glm::vec4 color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_data->BlueShader)->Bind();
		std::dynamic_pointer_cast<Forge::OpenGLShader>(s_data->BlueShader)->UploadUniformFloat3("u_Color", color);
		std::dynamic_pointer_cast<OpenGLShader>(s_data->BlueShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
		RenderCommands::DrawIndexed(s_data->SquareVA);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2 size, const glm::vec4 color)
	{
		Renderer2D::DrawQuad(glm::vec3(pos, 0.0f), size, color);
	}
}