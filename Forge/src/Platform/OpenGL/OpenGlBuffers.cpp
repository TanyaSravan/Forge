#include "FGpch.h"
#include "OpenGlBuffers.h"

#include "glad/glad.h"

namespace Forge {

	//VertexBuffer
	OpenGlVertexBuffers::OpenGlVertexBuffers(float* vertices, uint32_t size) {
		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGlVertexBuffers::~OpenGlVertexBuffers() {
		glDeleteBuffers(1, &m_RendererId);
	}

	void OpenGlVertexBuffers::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	}

	void OpenGlVertexBuffers::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//Index Buffer
	OpenGlIndexBuffers::OpenGlIndexBuffers(uint32_t* indices, uint32_t count)
		:m_Count(count) 
	{
		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGlIndexBuffers::~OpenGlIndexBuffers() {
		glDeleteBuffers(1, &m_RendererId);
	}

	void OpenGlIndexBuffers::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
	}

	void OpenGlIndexBuffers::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}