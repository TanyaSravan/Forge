#include "FGpch.h"
#include "OpenGlShader.h"

#include "glm/gtc/type_ptr.hpp"
#include "glad/glad.h" 
#include <fstream>

namespace Forge {
	static GLenum ShaderStringtoGLType(const std::string& type) {
		if (type == "Vertex")
			return GL_VERTEX_SHADER;
		else if (type == "Fragment" | type == "Pixel")
			return GL_FRAGMENT_SHADER;
		
		FG_CORE_ASSERT(false, "Unknown shader type");
	}

	OpenGLShader::OpenGLShader(const std::string& filepath) {
		std::string source = ReadShader(filepath);
		std::unordered_map<GLenum, std::string> sourceShader = ParseShader(source);
		CompileShader(sourceShader);

		// assets/Shader/Texture.glsl
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 1 : lastSlash + 1;
		auto lastDot = filepath.rfind(".");
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_name = filepath.substr(lastSlash, count);
	}

	std::string OpenGLShader::ReadShader(const std::string& filepath) {

		std::ifstream in(filepath, std::ios::in | std::ios::binary);

		std::string result;
		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else {
			FG_CORE_ERROR("Shader file is not valid {0}", filepath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::ParseShader(const std::string& source) {

		std::unordered_map<GLenum, std::string> sourceShader;

		const char* typeToken = "#type";
		size_t typeTokenLen = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos) {
			size_t eol = source.find_first_of("\r\n", pos);
			FG_CORE_ASSERT(eol, "Syntax Error");
			size_t begin = pos + typeTokenLen + 1;
			std::string type = source.substr(begin, eol - begin);
			FG_CORE_ASSERT(ShaderStringtoGLType(type), "Unknown Shader Type");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);

			sourceShader[ShaderStringtoGLType(type)] = source.substr(nextLinePos,
				pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return sourceShader;
	}

	void OpenGLShader::CompileShader(const std::unordered_map<GLenum, std::string>& sourceShader) {

		GLuint program = glCreateProgram();
		FG_CORE_ASSERT(sourceShader.size() <= 2, "We only support 2 shaders");
		std::array<GLenum,2> glShaderIds;
		int ArrayIndex = 0;

		for (auto& kv : sourceShader) {
			GLenum shaderType = kv.first;
			const std::string& shaderCode = kv.second;

			GLuint shader = glCreateShader(shaderType);
			const GLchar* source = shaderCode.c_str();
			glShaderSource(shader, 1, &source, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
				glDeleteShader(shader);

				FG_CORE_ERROR("{0}", infoLog.data());
				FG_CORE_ASSERT(false, "Compilation failed")
				break;
			}

			glAttachShader(program, shader);
			glShaderIds[ArrayIndex++] = shader;
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto& shaderIds : glShaderIds) {
				glDeleteShader(shaderIds);
			}

			// Use the infoLog as you see fit.
			FG_CORE_ERROR("{0}", infoLog.data());
			FG_CORE_ASSERT(false, "Program Linking Failed")
			return;
		}

		for (auto& shaderIds : glShaderIds) {
			glDetachShader(program, shaderIds);
		}

		m_RendererId = program;
	}

	OpenGLShader::OpenGLShader(const std::string& name,const std::string& vertexShaderSrc, const std::string& fragmentShaderSrc)
		:m_name(name)
	{
		std::unordered_map<GLenum, std::string> sourceShader;
		sourceShader[GL_VERTEX_SHADER] = vertexShaderSrc;
		sourceShader[GL_FRAGMENT_SHADER] = fragmentShaderSrc;

		CompileShader(sourceShader);

	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(m_RendererId);
	}

	void OpenGLShader::Bind() const {
		glUseProgram(m_RendererId);
	}
	
	void OpenGLShader::Unbind() const {
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) {
		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}


	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {
		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, const int& value) {
		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, const float& value) {
		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value) {
		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value) {
		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value) {
		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
}