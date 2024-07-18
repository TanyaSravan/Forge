#pragma once
#include "Forge/Renderer/Shader.h"

namespace Forge {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		uint32_t m_RendererId;
	};
}
