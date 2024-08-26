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
		Ref<Shader> TextureShader;
		Ref<Texture> DefaultTexture;
	};

	static Renderer2DStorage* s_data;

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		FG_PROFILE_FUNCTION();
		s_data->TextureShader->Bind();
		s_data->TextureShader->SetMat4("u_VP", camera.GetVPMatrix());
	}
	void Renderer2D::Init()
	{
		FG_PROFILE_FUNCTION();
		s_data = new Renderer2DStorage();
		s_data->SquareVA = VertexArray::Create();
		float Squarevertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,	1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,	1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f,
		};

		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(Squarevertices, sizeof(Squarevertices));

		BufferLayout Squarelayout = {
			{ShaderDataType::Float3, "position"},
			{ShaderDataType::Float2, "TexCoord"},
		};

		squareVB->SetLayout(Squarelayout);
		s_data->SquareVA->AddVertexBuffer(squareVB);

		unsigned int Squareindices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer>squareIB;
		squareIB = IndexBuffer::Create(Squareindices, 6);

		s_data->SquareVA->SetIndexBuffer(squareIB);

		s_data->TextureShader = Shader::Create("assets/Shaders/TextureShader.glsl");

		s_data->TextureShader->SetInt("u_Texture", 0);

		s_data->DefaultTexture = Texture2D::Create(1, 1);
		uint32_t WhiteTexData = 0xffffffff;
		s_data->DefaultTexture->SetData(&WhiteTexData, sizeof(uint32_t));
		
	}
	void Renderer2D::Shutdown()
	{
		delete s_data;
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2 size, const glm::vec4& color)
	{
		FG_PROFILE_FUNCTION();

		s_data->DefaultTexture->Bind();
		s_data->TextureShader->SetFloat("u_NumTiles", 1);

		s_data->TextureShader->SetFloat4("u_Color", color);

		glm::mat4 squareTransform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size,1.0f });
		s_data->TextureShader->SetMat4("u_Transform", squareTransform);

		s_data->SquareVA->Bind();
		RenderCommands::DrawIndexed(s_data->SquareVA);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2 size, const glm::vec4& color)
	{
		Renderer2D::DrawQuad(glm::vec3(pos, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2 size, const Ref<Texture>& texture, const glm::vec4& color)
	{
		FG_PROFILE_FUNCTION();

		texture->Bind();

		s_data->TextureShader->SetFloat4("u_Color", color);

		s_data->TextureShader->SetFloat("u_NumTiles", 1);

		glm::mat4 squareTransform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size,1.0f });
		s_data->TextureShader->SetMat4("u_Transform", squareTransform);

		RenderCommands::DrawIndexed(s_data->SquareVA);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2 size, const Ref<Texture>& texture, const glm::vec4& color)
	{
		Renderer2D::DrawQuad(glm::vec3(pos, 0.0f), size, texture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2 size, const Ref<Texture>& texture, const float& numTiles , const glm::vec4& color)
	{
		FG_PROFILE_FUNCTION();

		texture->Bind();

		s_data->TextureShader->SetFloat4("u_Color", color);

		s_data->TextureShader->SetFloat("u_NumTiles", numTiles);

		glm::mat4 squareTransform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size,1.0f });
		s_data->TextureShader->SetMat4("u_Transform", squareTransform);

		RenderCommands::DrawIndexed(s_data->SquareVA);
	}
	
	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2 size, const Ref<Texture>& texture, const float& numTiles, const glm::vec4& color)
	{
		Renderer2D::DrawQuad(glm::vec3(pos, 0.0f), size, texture, numTiles, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2 size, const float& rotate, const glm::vec4& color)
	{
		FG_PROFILE_FUNCTION();

		s_data->DefaultTexture->Bind();
		s_data->TextureShader->SetFloat("u_NumTiles", 1);

		s_data->TextureShader->SetFloat4("u_Color", color);

		glm::mat4 squareTransform = glm::translate(glm::mat4(1.0f), pos) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), { size,1.0f });
		s_data->TextureShader->SetMat4("u_Transform", squareTransform);

		s_data->SquareVA->Bind();
		RenderCommands::DrawIndexed(s_data->SquareVA);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2 size, const float& rotate, const glm::vec4& color)
	{
		Renderer2D::DrawRotatedQuad(glm::vec3(pos, 0.0f), size, rotate, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2 size, const float& rotate, const Ref<Texture>& texture, const glm::vec4& color)
	{
		FG_PROFILE_FUNCTION();

		texture->Bind();
		s_data->TextureShader->SetFloat("u_NumTiles", 1);

		s_data->TextureShader->SetFloat4("u_Color", color);

		glm::mat4 squareTransform = glm::translate(glm::mat4(1.0f), pos) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), { size,1.0f });
		s_data->TextureShader->SetMat4("u_Transform", squareTransform);

		s_data->SquareVA->Bind();
		RenderCommands::DrawIndexed(s_data->SquareVA);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2 size, const float& rotate, const Ref<Texture>& texture, const glm::vec4& color)
	{
		Renderer2D::DrawRotatedQuad(glm::vec3(pos, 0.0f), size, rotate, texture, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2 size, const float& rotate, const Ref<Texture>& texture, const float& numTiles, const glm::vec4& color)
	{
		FG_PROFILE_FUNCTION();

		texture->Bind();
		s_data->TextureShader->SetFloat("u_NumTiles", numTiles);

		s_data->TextureShader->SetFloat4("u_Color", color);

		glm::mat4 squareTransform = glm::translate(glm::mat4(1.0f), pos) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), { size,1.0f });
		s_data->TextureShader->SetMat4("u_Transform", squareTransform);

		s_data->SquareVA->Bind();
		RenderCommands::DrawIndexed(s_data->SquareVA);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2 size, const float& rotate, const Ref<Texture>& texture, const float& numTiles, const glm::vec4& color)
	{
		Renderer2D::DrawRotatedQuad(glm::vec3(pos, 0.0f), size, rotate, texture, numTiles, color);
	}
}