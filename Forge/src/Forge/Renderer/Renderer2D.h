#pragma once
#include "Camera.h"
#include "Texture.h"
#include "SubTextures2D.h"

namespace Forge {
	class Renderer2D {
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Flush();

		static void Init();
		static void Shutdown();

		static void DrawQuad(const glm::vec2& pos, const glm::vec2 size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2 size, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& pos, const glm::vec2 size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2 size, const Ref<Texture2D>& texture);

		static void DrawQuad(const glm::vec2& pos, const glm::vec2 size, const Ref<Texture2D>& texture, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2 size, const Ref<Texture2D>& texture, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& pos, const glm::vec2 size, const Ref<Texture2D>& texture, const float& numTiles);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2 size, const Ref<Texture2D>& texture, const float& numTiles);

		static void DrawQuad(const glm::vec2& pos, const glm::vec2 size, const Ref<Texture2D>& texture, const float& numTiles, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2 size, const Ref<Texture2D>& texture, const float& numTiles, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& pos, const glm::vec2 size, const Ref<SubTextures2D>& subTexture);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2 size, const Ref<SubTextures2D>& subTexture);
		static void DrawQuad(const glm::vec2& pos, const glm::vec2 size, const Ref<SubTextures2D>& subTexture, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2 size, const Ref<SubTextures2D>& subTexture, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& pos, const glm::vec2 size, const Ref<SubTextures2D>& subTexture, const float& numTiles);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2 size, const Ref<SubTextures2D>& subTexture, const float& numTiles);
		static void DrawQuad(const glm::vec2& pos, const glm::vec2 size, const Ref<SubTextures2D>& subTexture, const float& numTiles, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2 size, const Ref<SubTextures2D>& subTexture, const float& numTiles, const glm::vec4& color);

		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2 size, const float& rotate, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2 size, const float& rotate, const glm::vec4& color);	

		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2 size, const float& rotate, const Ref<Texture2D>& texture);
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2 size, const float& rotate, const Ref<Texture2D>& texture);

		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2 size, const float& rotate, const Ref<Texture2D>& texture, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2 size, const float& rotate, const Ref<Texture2D>& texture, const glm::vec4& color);

		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2 size, const float& rotate, const Ref<Texture2D>& texture, const float& numTiles);
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2 size, const float& rotate, const Ref<Texture2D>& texture, const float& numTiles);

		static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2 size, const float& rotate, const Ref<Texture2D>& texture, const float& numTiles, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2 size, const float& rotate, const Ref<Texture2D>& texture, const float& numTiles, const glm::vec4& color);

		struct RendererStats {
			uint32_t DrawCalls;
			uint32_t NumQuads;

			uint32_t GetNumVertices() { return NumQuads * 4; }
			uint32_t GetNumIndices() { return NumQuads * 6; }

		};

		static RendererStats GetStat();
		static void ResetStats();

	private:
		static void ResetAndFlush();

	};
}
