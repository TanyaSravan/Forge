#include "FGpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGlVertexArray.h"
namespace Forge {

	VertexArray* VertexArray::Create() {

		switch (Renderer::GetAPI())
		{
			case RenderAPI::None:		FG_CORE_ASSERT(false, "Currently does not support RendererAPI::None"); return nullptr;
			case RenderAPI::OpenGL:		return new OpenGlVertexArray();
		}
	}
}