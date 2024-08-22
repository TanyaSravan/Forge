#include "FGpch.h"
#include "Renderer.h"
#include "Renderer2D.h"

namespace Forge {

	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData();

	void Renderer::BeginScene(OrthographicCamera& camera) {
		s_SceneData->m_VPMatrix = camera.GetVPMatrix();
	}

	void Renderer::EndScene()
	{
		FG_PROFILE_FUNCTION();
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {

		shader->Bind();
		shader->SetMat4("u_VP", s_SceneData->m_VPMatrix);
		shader->SetMat4("u_Transform", transform);
		RenderCommands::DrawIndexed(vertexArray);
	}

	void Renderer::Init() {
		FG_PROFILE_FUNCTION();
		RenderCommands::Init();
		Renderer2D::Init();
	}

	void Renderer::SetViewport(uint32_t width, uint32_t height) {
		RenderCommands::SetViewport(0, 0, width, height);
	}
}