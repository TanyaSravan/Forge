#pragma once
#include "RenderCommands.h"

namespace Forge {

	class Renderer {
	public:
		static void BeginScene() {};
		static void EndScene() {};

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray) {
			RenderCommands::DrawIndexed(vertexArray);
		}

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}
