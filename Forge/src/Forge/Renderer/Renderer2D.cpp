#include "FGpch.h"
#include "Renderer2D.h"
#include "RenderCommands.h"
#include "VertexArray.h"
#include "Shader.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Forge {

	struct QuadVertex {
		glm::vec3 Pos;
		glm::vec4 Color;
		glm::vec2 TexCoord;
	};

	struct Renderer2DStorage {
		Ref<VertexArray> SquareVA;
		Ref<VertexBuffer> squareVB;
		Ref<Shader> TextureShader;
		Ref<Texture> DefaultTexture;

		uint32_t MaxQuads = 10000;
		uint32_t MaxVertices = MaxQuads * 4;
		uint32_t MaxIndices = MaxQuads * 6;

		QuadVertex* QuadVertexBase = nullptr;
		QuadVertex* QuadVertexptr = nullptr;

		uint32_t QuadIndexCount;
	};

	static Renderer2DStorage s_data;

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		FG_PROFILE_FUNCTION();
		s_data.TextureShader->Bind();
		s_data.TextureShader->SetMat4("u_VP", camera.GetVPMatrix());

		s_data.QuadVertexptr = s_data.QuadVertexBase;
		s_data.QuadIndexCount = 0;
	}
	void Renderer2D::EndScene()
	{
		FG_PROFILE_FUNCTION();
		uint32_t size = (uint8_t*)s_data.QuadVertexptr - (uint8_t*)s_data.QuadVertexBase;
		s_data.squareVB->SetData(s_data.QuadVertexBase, size);
		Renderer2D::Flush();

	}
	void Renderer2D::Flush()
	{
		RenderCommands::DrawIndexed(s_data.SquareVA, s_data.QuadIndexCount);
	}

	void Renderer2D::Init()
	{
		FG_PROFILE_FUNCTION();
		s_data.SquareVA = VertexArray::Create();

		s_data.QuadVertexBase = new QuadVertex[s_data.MaxVertices];

		s_data.squareVB = VertexBuffer::Create(sizeof(QuadVertex)* s_data.MaxVertices);

		BufferLayout Squarelayout = {
			{ShaderDataType::Float3, "Position"},
			{ShaderDataType::Float4, "Color"},
			{ShaderDataType::Float2, "TexCoord"},
		};

		s_data.squareVB->SetLayout(Squarelayout);
		s_data.SquareVA->AddVertexBuffer(s_data.squareVB);

		uint32_t* indices = new uint32_t[s_data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_data.MaxIndices; i += 6) {
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer>squareIB;
		squareIB = IndexBuffer::Create(indices, s_data.MaxIndices);
		delete[] indices;

		s_data.SquareVA->SetIndexBuffer(squareIB);

		s_data.TextureShader = Shader::Create("assets/Shaders/TextureShader.glsl");

		s_data.TextureShader->SetInt("u_Texture", 0);

		s_data.DefaultTexture = Texture2D::Create(1, 1);
		uint32_t WhiteTexData = 0xffffffff;
		s_data.DefaultTexture->SetData(&WhiteTexData, sizeof(uint32_t));
		
	}
	void Renderer2D::Shutdown()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2 size, const glm::vec4& color)
	{
		FG_PROFILE_FUNCTION();
		s_data.QuadVertexptr->Pos = pos;
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,0.0f };
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = { pos.x + size.x , pos.y, pos.z};
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f,0.0f };
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = { pos.x + size.x , pos.y + size.y, pos.z };
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f, 1.0f };
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = { pos.x, pos.y + size.y, pos.z };
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,1.0f };
		s_data.QuadVertexptr++;

		s_data.QuadIndexCount += 6;

		/*
		s_data.DefaultTexture->Bind();
		s_data.TextureShader->SetFloat("u_NumTiles", 1);

		glm::mat4 squareTransform = glm::mat4(1.0f);
		squareTransform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size,1.0f });
		s_data.TextureShader->SetMat4("u_Transform", squareTransform);

		s_data.SquareVA->Bind();
		RenderCommands::DrawIndexed(s_data.SquareVA);
		*/
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2 size, const glm::vec4& color)
	{
		Renderer2D::DrawQuad(glm::vec3(pos, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2 size, const Ref<Texture>& texture, const glm::vec4& color)
	{
		FG_PROFILE_FUNCTION();

		texture->Bind();

		s_data.TextureShader->SetFloat4("u_Color", color);

		s_data.TextureShader->SetFloat("u_NumTiles", 1);

		glm::mat4 squareTransform = glm::mat4(1.0f);
		squareTransform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size,1.0f });
		s_data.TextureShader->SetMat4("u_Transform", squareTransform);

		RenderCommands::DrawIndexed(s_data.SquareVA);
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2 size, const Ref<Texture>& texture, const glm::vec4& color)
	{
		Renderer2D::DrawQuad(glm::vec3(pos, 0.0f), size, texture, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2 size, const Ref<Texture>& texture, const float& numTiles , const glm::vec4& color)
	{
		FG_PROFILE_FUNCTION();

		texture->Bind();

		s_data.TextureShader->SetFloat4("u_Color", color);

		s_data.TextureShader->SetFloat("u_NumTiles", numTiles);

		glm::mat4 squareTransform = glm::mat4(1.0f);
		squareTransform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size,1.0f });
		s_data.TextureShader->SetMat4("u_Transform", squareTransform);

		RenderCommands::DrawIndexed(s_data.SquareVA);
	}
	
	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2 size, const Ref<Texture>& texture, const float& numTiles, const glm::vec4& color)
	{
		Renderer2D::DrawQuad(glm::vec3(pos, 0.0f), size, texture, numTiles, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2 size, const float& rotate, const glm::vec4& color)
	{
		FG_PROFILE_FUNCTION();

		s_data.DefaultTexture->Bind();
		s_data.TextureShader->SetFloat("u_NumTiles", 1);

		s_data.TextureShader->SetFloat4("u_Color", color);

		glm::mat4 squareTransform = glm::mat4(1.0f);
		glm::translate(glm::mat4(1.0f), pos) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), { size,1.0f });
		s_data.TextureShader->SetMat4("u_Transform", squareTransform);

		s_data.SquareVA->Bind();
		RenderCommands::DrawIndexed(s_data.SquareVA);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2 size, const float& rotate, const glm::vec4& color)
	{
		Renderer2D::DrawRotatedQuad(glm::vec3(pos, 0.0f), size, rotate, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2 size, const float& rotate, const Ref<Texture>& texture, const glm::vec4& color)
	{
		FG_PROFILE_FUNCTION();

		texture->Bind();
		s_data.TextureShader->SetFloat("u_NumTiles", 1);

		s_data.TextureShader->SetFloat4("u_Color", color);

		glm::mat4 squareTransform = glm::mat4(1.0f);
		squareTransform = glm::translate(glm::mat4(1.0f), pos) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), { size,1.0f });
		s_data.TextureShader->SetMat4("u_Transform", squareTransform);

		s_data.SquareVA->Bind();
		RenderCommands::DrawIndexed(s_data.SquareVA);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2 size, const float& rotate, const Ref<Texture>& texture, const glm::vec4& color)
	{
		Renderer2D::DrawRotatedQuad(glm::vec3(pos, 0.0f), size, rotate, texture, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2 size, const float& rotate, const Ref<Texture>& texture, const float& numTiles, const glm::vec4& color)
	{
		FG_PROFILE_FUNCTION();

		texture->Bind();
		s_data.TextureShader->SetFloat("u_NumTiles", numTiles);

		s_data.TextureShader->SetFloat4("u_Color", color);

		glm::mat4 squareTransform = glm::mat4(1.0f);
		squareTransform = glm::translate(glm::mat4(1.0f), pos) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)) *
			glm::scale(glm::mat4(1.0f), { size,1.0f });
		s_data.TextureShader->SetMat4("u_Transform", squareTransform);

		s_data.SquareVA->Bind();
		RenderCommands::DrawIndexed(s_data.SquareVA);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2 size, const float& rotate, const Ref<Texture>& texture, const float& numTiles, const glm::vec4& color)
	{
		Renderer2D::DrawRotatedQuad(glm::vec3(pos, 0.0f), size, rotate, texture, numTiles, color);
	}
}