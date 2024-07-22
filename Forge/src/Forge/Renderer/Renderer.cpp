#include "FGpch.h"
#include "Renderer.h"

namespace Forge {

	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData();

	void Renderer::BeginScene(OrthographicCamera& camera) {
		s_SceneData->m_VPMatrix = camera.GetVPMatrix();
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform) {
		shader->Bind();
		shader->UploadUniformMat4("u_VP", s_SceneData->m_VPMatrix);
		shader->UploadUniformMat4("u_Transform", transform);
		RenderCommands::DrawIndexed(vertexArray);
	}
}