#include "FGpch.h"
#include "OpenGlVertexArray.h"
#include "glad/glad.h"

namespace Forge {

	static GLenum GetShaderDataTypetoOpenGLBaseType(const ShaderDataType& type) {
		switch (type)
		{
			case Forge::ShaderDataType::None:			FG_CORE_ASSERT(false, "Does notsupport none ShaderType"); return GL_NONE;
			case Forge::ShaderDataType::Float:			return GL_FLOAT;
			case Forge::ShaderDataType::Float2:			return GL_FLOAT;
			case Forge::ShaderDataType::Float3:			return GL_FLOAT;
			case Forge::ShaderDataType::Float4:			return GL_FLOAT;
			case Forge::ShaderDataType::Int:			return GL_INT;
			case Forge::ShaderDataType::Int2:			return GL_INT;
			case Forge::ShaderDataType::Int3:			return GL_INT;
			case Forge::ShaderDataType::Int4:			return GL_INT;
			case Forge::ShaderDataType::Mat3:			return GL_FLOAT;
			case Forge::ShaderDataType::Mat4:			return GL_FLOAT;
			case Forge::ShaderDataType::Bool:			return GL_BOOL;
		}

		FG_CORE_ASSERT(false, "Unknown Shader DataType");
		return 0;
	}
	
	OpenGlVertexArray::OpenGlVertexArray() {
		FG_PROFILE_FUNCTION();
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGlVertexArray::~OpenGlVertexArray() {
		FG_PROFILE_FUNCTION();
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGlVertexArray::Bind() const {
		FG_PROFILE_FUNCTION();
		glBindVertexArray(m_RendererID);
	}

	void OpenGlVertexArray::Unbind() const {
		FG_PROFILE_FUNCTION();
		glBindVertexArray(0);
	}

	void OpenGlVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
		FG_PROFILE_FUNCTION();
		FG_CORE_ASSERT(vertexBuffer->GetLayout().GetBufferElements().size(), "The Vertex Layout is not set")

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		unsigned int index = 0;

		for (auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetElementCount(element.Type),
				GetShaderDataTypetoOpenGLBaseType(element.Type),
				element.Normalised ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGlVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
		FG_PROFILE_FUNCTION();
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}
