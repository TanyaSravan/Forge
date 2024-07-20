#pragma once
#include "Buffers.h"
#include <memory>

namespace Forge {
	class VertexArray {
	public:

		virtual ~VertexArray() {}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		virtual std::vector<std::shared_ptr<VertexBuffer>> GetVertexBuffers() const = 0;
		virtual std::shared_ptr<IndexBuffer> GetIndexBuffers() const = 0;

		static VertexArray* Create();
	};
}