#pragma once
#include "RenderCommands.h"
#include "Camera.h"
#include "Shader.h"

namespace Forge {

	class Renderer {
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene() {};

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData {
			glm::mat4 m_VPMatrix;
		};
	private:
		static SceneData* s_SceneData;
	};
}
