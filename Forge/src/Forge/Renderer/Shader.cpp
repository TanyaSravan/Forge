#include "FGpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGlShader.h"

#include "glad/glad.h"


namespace Forge {
	Ref<Shader> Shader::Create(const std::string& VertexShaderSrc, const std::string& FragmentShaderSrc) {

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		FG_CORE_ASSERT(false, "Currently does not support RendererAPI::None"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(VertexShaderSrc, FragmentShaderSrc);
		}
	}

	Ref<Shader> Shader::Create(const std::string& filepath) {

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		FG_CORE_ASSERT(false, "Currently does not support RendererAPI::None"); return nullptr;
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLShader>(filepath);
		}
	}
}