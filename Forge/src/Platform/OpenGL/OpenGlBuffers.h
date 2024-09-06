#pragma once

#include <cstdint>
#include "Forge/Renderer/Buffers.h"
namespace Forge {
	
	class OpenGlVertexBuffers : public VertexBuffer {
	public:
		OpenGlVertexBuffers(uint32_t size);
		OpenGlVertexBuffers(float* vertices, uint32_t size);

		~OpenGlVertexBuffers();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* VertexBuffer, uint32_t size) override;

		virtual void SetLayout(BufferLayout layout) override { m_Layout = layout; }
		virtual const BufferLayout GetLayout() const override { return m_Layout; }

	private:
		uint32_t m_RendererId;
		BufferLayout m_Layout;
		
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
