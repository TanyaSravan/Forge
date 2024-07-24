#pragma once
#include "Forge/Renderer/VertexArray.h"

namespace Forge {

	class OpenGlVertexArray : public VertexArray {
	public:
		OpenGlVertexArray();
		~OpenGlVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual std::vector<Ref<VertexBuffer>> GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual Ref<IndexBuffer> GetIndexBuffers() const override { return m_IndexBuffer; }

	private:
		uint32_t m_RendererID;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}
