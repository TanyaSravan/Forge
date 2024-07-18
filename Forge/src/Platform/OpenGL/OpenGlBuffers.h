#pragma once

#include <cstdint>
#include "Forge/Renderer/Buffers.h"
namespace Forge {
	
	class OpenGlVertexBuffers : public VertexBuffer {
	public:
		OpenGlVertexBuffers(float* vertices, uint32_t size);

		~OpenGlVertexBuffers();

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		uint32_t m_RendererId;
		
	};

	class OpenGlIndexBuffers : public IndexBuffer {
	public:
		OpenGlIndexBuffers(uint32_t* indices, uint32_t count);
		~OpenGlIndexBuffers();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual uint32_t GetCount() const override { return m_Count; }

	private:
		uint32_t m_RendererId;
		uint32_t m_Count;

	};
}
