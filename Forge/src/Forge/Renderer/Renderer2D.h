#pragma once
#include "Camera.h"

namespace Forge {
	class Renderer2D {
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene() {};

		static void Init();
		static void Shutdown();

		static void DrawQuad(const glm::vec2& pos, const glm::vec2 size, const glm::vec4 color);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2 size, const glm::vec4 color);
	};
}
