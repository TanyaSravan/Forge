#pragma once
#include "Camera.h"
#include "Texture.h"

namespace Forge {
	class Renderer2D {
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene() {};

		static void Init();
		static void Shutdown();

		static void DrawQuad(const glm::vec2& pos, const glm::vec2 size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2 size, const glm::vec4& color);

		static void DrawQuad(const glm::vec2& pos, const glm::vec2 size, const Ref<Texture>& texture, const float& numTiles = 1, const glm::vec4& color = { 1.0f,1.0f,1.0f,1.0f });
		static void DrawQuad(const glm::vec3& pos, const glm::vec2 size, const Ref<Texture>& texture, const float& numTiles = 1, const glm::vec4& color = { 1.0f,1.0f,1.0f,1.0f });
	};
}
