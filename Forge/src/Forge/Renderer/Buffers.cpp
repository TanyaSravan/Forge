#include "FGpch.h"
#include "Buffers.h"
#include "Forge/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGlBuffers.h"

namespace Forge{

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		FG_CORE_ASSERT(false, "Currently does not support RendererAPI::None"); return nullptr;
			case RendererAPI::API::OpenGL:		return new OpenGlVertexBuffers(vertices, size);
		}


	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		FG_CORE_ASSERT(false, "Currently does not support RendererAPI::None"); return nullptr;
			case RendererAPI::API::OpenGL:		return new OpenGlIndexBuffers(indices, size);
		}
	}
}