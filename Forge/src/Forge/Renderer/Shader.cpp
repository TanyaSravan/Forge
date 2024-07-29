#include "FGpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGlShader.h"

#include "glad/glad.h"


namespace Forge {
	Ref<Shader> Shader::Create(const std::string& name,const std::string& VertexShaderSrc, const std::string& FragmentShaderSrc) {

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		FG_CORE_ASSERT(false, "Currently does not support RendererAPI::None"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(name,VertexShaderSrc, FragmentShaderSrc);
		}
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& filepath) {

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		FG_CORE_ASSERT(false, "Currently does not support RendererAPI::None"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(filepath);
		}
		return nullptr;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name) {
		FG_CORE_ASSERT(Exists(name), "Shader name does not exist");
		return m_shaders[name];
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader) {
		FG_CORE_ASSERT(!Exists(name), "Shader name already exist")
		m_shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader) {
		std::string name = shader->GetName();
		Add(name, shader);
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath) {
		Ref<Shader> shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string & filepath) {
		Ref<Shader> shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	bool ShaderLibrary::Exists(const std::string& name) const {
		return m_shaders.find(name) != m_shaders.end();
	}


}