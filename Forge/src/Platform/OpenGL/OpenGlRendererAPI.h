#pragma once
#include "Forge/Renderer/RendererAPI.h"

namespace Forge {
	class OpenGlRendererAPI :public RendererAPI {

		virtual void Clear() override;
		virtual void SetClearColor(const glm::vec4& color) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};

}
