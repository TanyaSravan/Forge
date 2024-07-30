#include "FGpch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Platform/OpenGL/OpenGlShader.h"

namespace Forge {

	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData();

	void Renderer::BeginScene(OrthographicCamera& camera) {
		s_SceneData->m_VPMatrix = camera.GetVPMatrix();
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {

		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_VP", s_SceneData->m_VPMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
		RenderCommands::DrawIndexed(vertexArray);
	}

	void Renderer::Init() {
		RenderCommands::Init();
		Renderer2D::Init();
	}

	void Renderer::SetViewport(uint32_t width, uint32_t height) {
		RenderCommands::SetViewport(0, 0, width, height);
	}
}