#pragma once
#include "Forge/Renderer/RendererAPI.h"

namespace Forge {
	class OpenGlRendererAPI :public RendererAPI {

		virtual void Clear() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
		virtual void Init() override;
	};

}
