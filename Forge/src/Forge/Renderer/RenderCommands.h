#pragma once
#include "RendererAPI.h"

namespace Forge {
	class RenderCommands {
	public:

		inline static void SetClearColor(const glm::vec4& color) {
			s_RendererAPI->SetClearColor(color);
		}

		inline static void SetViewport(uint32_t x,uint32_t y, uint32_t width, uint32_t height) {
			s_RendererAPI->SetViewport(x,y,width,height);
		}

		inline static void Clear() {
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t IndexCount = 0) {
			s_RendererAPI->DrawIndexed(vertexArray, IndexCount);
		}

		inline static void Init() {
			s_RendererAPI->Init();
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}
