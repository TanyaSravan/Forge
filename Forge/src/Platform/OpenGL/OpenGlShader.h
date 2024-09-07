#pragma once
#include "Forge/Renderer/Shader.h"

typedef unsigned int GLenum;

namespace Forge {
	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& name, const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc);
		OpenGLShader(const std::string& filepath);
		~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetMat4(const std::string& name, const glm::mat4& value) 	override;
		virtual void SetFloat(const std::string& name, const float& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetInt(const std::string& name, const int& value) override;
		virtual void SetIntArray(const std::string& name, const int32_t* values, uint32_t count) override;

		virtual std::string GetName() const override { return m_name; }

		virtual void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		virtual void UploadUniformInt(const std::string& name, const int& value);
		virtual void UploadUniformIntArray(const std::string& name, const int* values, uint32_t count);

		virtual void UploadUniformFloat(const std::string& name, const float& value);
		virtual void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

	private:
		void CompileShader(const std::unordered_map<GLenum,std::string>& sourceShader);
		std::unordered_map<GLenum, std::string> ParseShader(const std::string& source);
		std::string ReadShader(const std::string& filepath);
	private:
		uint32_t m_RendererId;
		std::string m_name;
	};
}
