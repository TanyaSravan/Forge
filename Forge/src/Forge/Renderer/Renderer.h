#pragma once
#include "RenderCommands.h"
#include "Camera.h"
#include "Shader.h"

namespace Forge {

	class Renderer {
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Init();
		static void SetViewport(uint32_t width, uint32_t height);

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			glm::mat4 m_VPMatrix;
		};
	private:
		static SceneData* s_SceneData;
	};
}
