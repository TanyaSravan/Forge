#pragma once
#include "Forge/Renderer/Shader.h"

namespace Forge {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		virtual void UploadUniformInt(const std::string& name, const int& value);

		virtual void UploadUniformFloat(const std::string& name, const float& value);
		virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
	private:
		uint32_t m_RendererId;
	};
}
