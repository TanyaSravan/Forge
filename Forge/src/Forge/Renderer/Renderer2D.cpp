#include "FGpch.h"
#include "Renderer2D.h"
#include "RenderCommands.h"
#include "VertexArray.h"
#include "Shader.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Forge {

	struct Renderer2DStorage {
		Ref<VertexArray> SquareVA;
		Ref<Shader> BlueShader;
	};

	static Renderer2DStorage* s_data;

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		s_data->BlueShader->Bind();
		s_data->BlueShader->SetMat4("u_VP", camera.GetVPMatrix());
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
		s_data->BlueShader->Bind();
		s_data->BlueShader->SetFloat3("u_Color", color);

		glm::mat4 squareTransform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size,1.0f });

		s_data->BlueShader->SetMat4("u_Transform", squareTransform);
		RenderCommands::DrawIndexed(s_data->SquareVA);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2 size, const glm::vec4 color)
	{
		Renderer2D::DrawQuad(glm::vec3(pos, 0.0f), size, color);
	}
}