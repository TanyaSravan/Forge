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
		float TexIndex;
		float Numtiles;
	};

	struct Renderer2DStorage {
		Ref<VertexArray> SquareVA;
		Ref<VertexBuffer> squareVB;
		Ref<Shader> TextureShader;
		Ref<Texture2D> DefaultTexture;

		uint32_t MaxQuads = 10000;
		uint32_t MaxVertices = MaxQuads * 4;
		uint32_t MaxIndices = MaxQuads * 6;

		QuadVertex* QuadVertexBase = nullptr;
		QuadVertex* QuadVertexptr = nullptr;

		uint32_t QuadIndexCount;

		static const uint32_t MaxTextureSlots = 32;
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		glm::vec4 QuadVertices[4];
		
		Renderer2D::RendererStats stats;
	};

	static Renderer2DStorage s_data;

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		FG_PROFILE_FUNCTION();
		s_data.TextureShader->Bind();
		s_data.TextureShader->SetMat4("u_VP", camera.GetVPMatrix());

		s_data.QuadVertexptr = s_data.QuadVertexBase;
		s_data.QuadIndexCount = 0;
		s_data.TextureSlotIndex = 1;
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
		for (uint32_t i = 0; i < s_data.TextureSlotIndex; i++) {
			s_data.TextureSlots[i]->Bind(i);
		}
		RenderCommands::DrawIndexed(s_data.SquareVA, s_data.QuadIndexCount);
		s_data.stats.DrawCalls++;
	}

	void Renderer2D::ResetAndFlush()
	{
		Renderer2D::EndScene();
		s_data.QuadVertexptr = s_data.QuadVertexBase;
		s_data.QuadIndexCount = 0;
		s_data.TextureSlotIndex = 1;
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
			{ShaderDataType::Float, "TexIndex"},
			{ShaderDataType::Float, "NumTiles"},
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

		int32_t samplers[s_data.MaxTextureSlots];
		for (int32_t i = 0; i < s_data.MaxTextureSlots; i++) {
			samplers[i] = i;
		}

		s_data.TextureShader = Shader::Create("assets/Shaders/TextureShader.glsl");
		s_data.TextureShader->Bind();
		s_data.TextureShader->SetIntArray("u_Texture", samplers, s_data.MaxTextureSlots);

		s_data.DefaultTexture = Texture2D::Create(1, 1);
		uint32_t WhiteTexData = 0xffffffff;
		s_data.DefaultTexture->SetData(&WhiteTexData, sizeof(uint32_t));

		s_data.TextureSlots[0] = s_data.DefaultTexture;

		s_data.QuadVertices[0] = { -0.5f,-0.5f,0.0f,1.0f };
		s_data.QuadVertices[1] = {  0.5f,-0.5f,0.0f,1.0f };
		s_data.QuadVertices[2] = {  0.5f, 0.5f,0.0f,1.0f };
		s_data.QuadVertices[3] = { -0.5f, 0.5f,0.0f,1.0f };
	}
	void Renderer2D::Shutdown()
	{
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2 size, const glm::vec4& color)
	{
		FG_PROFILE_FUNCTION();

		if (s_data.QuadIndexCount >= s_data.MaxIndices) {
			Renderer2D::ResetAndFlush();
		}

		float textureIndex = 0.0f;
		float numTiles = 1.0f;

		s_data.QuadVertexptr->Pos = pos;
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = { pos.x + size.x , pos.y, pos.z};
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = { pos.x + size.x , pos.y + size.y, pos.z };
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f, 1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = { pos.x, pos.y + size.y, pos.z };
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadIndexCount += 6;

		s_data.stats.NumQuads++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2 size, const glm::vec4& color)
	{
		Renderer2D::DrawQuad(glm::vec3(pos, 0.0f), size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2 size, const Ref<Texture2D>& texture)
	{
		FG_PROFILE_FUNCTION();

		if (s_data.QuadIndexCount >= s_data.MaxIndices) {
			Renderer2D::ResetAndFlush();
		}

		float numTiles = 1.0f;
		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_data.TextureSlotIndex; i++) {
			if (*s_data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0) {
			textureIndex = (float)s_data.TextureSlotIndex;
			s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
			s_data.TextureSlotIndex++;
		}

		constexpr glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };
		
		s_data.QuadVertexptr->Pos = pos;
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = { pos.x + size.x , pos.y, pos.z };
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = { pos.x + size.x , pos.y + size.y, pos.z };
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f, 1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = { pos.x, pos.y + size.y, pos.z };
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadIndexCount += 6;
		s_data.stats.NumQuads++;

	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2 size, const Ref<Texture2D>& texture)
	{
		Renderer2D::DrawQuad(glm::vec3(pos, 0.0f), size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2 size, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		FG_PROFILE_FUNCTION();

		if (s_data.QuadIndexCount >= s_data.MaxIndices) {
			Renderer2D::ResetAndFlush();
		}

		float numTiles = 1.0f;
		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_data.TextureSlotIndex; i++) {
			if (*s_data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0) {
			textureIndex = (float)s_data.TextureSlotIndex;
			s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
			s_data.TextureSlotIndex++;
		}

		s_data.QuadVertexptr->Pos = pos;
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = { pos.x + size.x , pos.y, pos.z };
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = { pos.x + size.x , pos.y + size.y, pos.z };
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f, 1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = { pos.x, pos.y + size.y, pos.z };
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadIndexCount += 6;
		s_data.stats.NumQuads++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2 size, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		Renderer2D::DrawQuad(glm::vec3(pos, 0.0f), size, texture,color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2 size, const Ref<Texture2D>& texture, const float& numTiles)
	{
		FG_PROFILE_FUNCTION();

		if (s_data.QuadIndexCount >= s_data.MaxIndices) {
			Renderer2D::ResetAndFlush();
		}

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_data.TextureSlotIndex; i++) {
			if (*s_data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0) {
			textureIndex = (float)s_data.TextureSlotIndex;
			s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
			s_data.TextureSlotIndex++;
		}

		constexpr glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };

		s_data.QuadVertexptr->Pos = pos;
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = { pos.x + size.x , pos.y, pos.z };
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = { pos.x + size.x , pos.y + size.y, pos.z };
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f, 1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = { pos.x, pos.y + size.y, pos.z };
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadIndexCount += 6;
		s_data.stats.NumQuads++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2 size, const Ref<Texture2D>& texture, const float& numTiles)
	{
		Renderer2D::DrawQuad(glm::vec3(pos, 0.0f), size, texture, numTiles);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2 size, const Ref<Texture2D>& texture, const float& numTiles , const glm::vec4& color)
	{
		FG_PROFILE_FUNCTION();

		if (s_data.QuadIndexCount >= s_data.MaxIndices) {
			Renderer2D::ResetAndFlush();
		}

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_data.TextureSlotIndex; i++) {
			if (*s_data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0) {
			textureIndex = (float)s_data.TextureSlotIndex;
			s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
			s_data.TextureSlotIndex++;
		}

		s_data.QuadVertexptr->Pos = pos;
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = { pos.x + size.x , pos.y, pos.z };
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = { pos.x + size.x , pos.y + size.y, pos.z };
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f, 1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = { pos.x, pos.y + size.y, pos.z };
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadIndexCount += 6;
		s_data.stats.NumQuads++;

	}

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2 size, const Ref<Texture2D>& texture, const float& numTiles, const glm::vec4& color)
	{
		Renderer2D::DrawQuad(glm::vec3(pos, 0.0f), size, texture, numTiles, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2 size, const float& rotate, const glm::vec4& color)
	{
		FG_PROFILE_FUNCTION();

		if (s_data.QuadIndexCount >= s_data.MaxIndices) {
			Renderer2D::ResetAndFlush();
		}

		glm::mat4 squareTransform = glm::mat4(1.0f);
		squareTransform = glm::translate(glm::mat4(1.0f), pos)*
			glm::rotate(glm::mat4(1.0f), glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f))*
			glm::scale(glm::mat4(1.0f), { size,1.0f });

		float textureIndex = 0.0f;
		float numTiles = 1.0f;

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[0];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[1];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[2];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f, 1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[3];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadIndexCount += 6;
		s_data.stats.NumQuads++;
	}


	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2 size, const float& rotate, const glm::vec4& color)
	{
		Renderer2D::DrawRotatedQuad(glm::vec3(pos, 0.0f), size, rotate, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2 size, const float& rotate, const Ref<Texture2D>& texture)
	{
		FG_PROFILE_FUNCTION();

		if (s_data.QuadIndexCount >= s_data.MaxIndices) {
			Renderer2D::ResetAndFlush();
		}

		glm::mat4 squareTransform = glm::mat4(1.0f);
		squareTransform = glm::translate(glm::mat4(1.0f), pos)*
			glm::rotate(glm::mat4(1.0f), glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f))*
			glm::scale(glm::mat4(1.0f), { size,1.0f });

		float textureIndex = 0.0f;
		float numTiles = 1.0f;


		for (uint32_t i = 1; i < s_data.TextureSlotIndex; i++) {
			if (*s_data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0) {
			textureIndex = (float)s_data.TextureSlotIndex;
			s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
			s_data.TextureSlotIndex++;
		}

		constexpr glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[0];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[1];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[2];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f, 1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[3];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadIndexCount += 6;
		s_data.stats.NumQuads++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2 size, const float& rotate, const Ref<Texture2D>& texture)
	{
		Renderer2D::DrawRotatedQuad(glm::vec3(pos, 0.0f), size, rotate, texture);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2 size, const float& rotate, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		FG_PROFILE_FUNCTION();

		if (s_data.QuadIndexCount >= s_data.MaxIndices) {
			Renderer2D::ResetAndFlush();
		}

		glm::mat4 squareTransform = glm::mat4(1.0f);
		squareTransform = glm::translate(glm::mat4(1.0f), pos)*
			glm::rotate(glm::mat4(1.0f), glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f))*
			glm::scale(glm::mat4(1.0f), { size,1.0f });

		float textureIndex = 0.0f;
		float numTiles = 1.0f;


		for (uint32_t i = 1; i < s_data.TextureSlotIndex; i++) {
			if (*s_data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0) {
			textureIndex = (float)s_data.TextureSlotIndex;
			s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
			s_data.TextureSlotIndex++;
		}

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[0];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[1];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[2];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f, 1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[3];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadIndexCount += 6;
		s_data.stats.NumQuads++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2 size, const float& rotate, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		Renderer2D::DrawRotatedQuad(glm::vec3(pos, 0.0f), size, rotate, texture, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2 size, const float& rotate, const Ref<Texture2D>& texture, const float& numTiles)
	{
		FG_PROFILE_FUNCTION();

		if (s_data.QuadIndexCount >= s_data.MaxIndices) {
			Renderer2D::ResetAndFlush();
		}

		glm::mat4 squareTransform = glm::mat4(1.0f);
		squareTransform = glm::translate(glm::mat4(1.0f), pos)*
			glm::rotate(glm::mat4(1.0f), glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f))*
			glm::scale(glm::mat4(1.0f), { size, 1.0f });

		float textureIndex = 0.0f;


		for (uint32_t i = 1; i < s_data.TextureSlotIndex; i++) {
			if (*s_data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0) {
			textureIndex = (float)s_data.TextureSlotIndex;
			s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
			s_data.TextureSlotIndex++;
		}

		constexpr glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[0];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[1];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[2];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f, 1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[3];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadIndexCount += 6;
		s_data.stats.NumQuads++;
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2 size, const float& rotate, const Ref<Texture2D>& texture, const float& numTiles)
	{
		Renderer2D::DrawRotatedQuad(glm::vec3(pos, 0.0f), size, rotate, texture, numTiles);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2 size, const float& rotate, const Ref<Texture2D>& texture, const float& numTiles, const glm::vec4& color)
	{
		FG_PROFILE_FUNCTION();

		if (s_data.QuadIndexCount >= s_data.MaxIndices) {
			Renderer2D::ResetAndFlush();
		}

		glm::mat4 squareTransform = glm::mat4(1.0f);
		squareTransform =  glm::translate(glm::mat4(1.0f), pos)*
			glm::rotate(glm::mat4(1.0f), glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f))*
			glm::scale(glm::mat4(1.0f), { size,1.0f });

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_data.TextureSlotIndex; i++) {
			if (*s_data.TextureSlots[i].get() == *texture.get()) {
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0) {
			textureIndex = (float)s_data.TextureSlotIndex;
			s_data.TextureSlots[s_data.TextureSlotIndex] = texture;
			s_data.TextureSlotIndex++;
		}

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[0];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[1];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f,0.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[2];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 1.0f, 1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadVertexptr->Pos = squareTransform * s_data.QuadVertices[3];
		s_data.QuadVertexptr->Color = color;
		s_data.QuadVertexptr->TexCoord = { 0.0f,1.0f };
		s_data.QuadVertexptr->TexIndex = textureIndex;
		s_data.QuadVertexptr->Numtiles = numTiles;
		s_data.QuadVertexptr++;

		s_data.QuadIndexCount += 6;
		s_data.stats.NumQuads++;
	}


	void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2 size, const float& rotate, const Ref<Texture2D>& texture, const float& numTiles, const glm::vec4& color)
	{
		Renderer2D::DrawRotatedQuad(glm::vec3(pos, 0.0f), size, rotate, texture, numTiles, color);
	}

	Renderer2D::RendererStats Renderer2D::GetStat()
	{
		return s_data.stats;
	}

	void Renderer2D::ResetStats()
	{
		s_data.stats.DrawCalls = 0;
		s_data.stats.NumQuads = 0;
	}

}