#include "FGpch.h"
#include "OpenGlRendererAPI.h"
#include "glad/glad.h"

namespace Forge {

	void OpenGlRendererAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGlRendererAPI::SetClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGlRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray) {
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffers()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}