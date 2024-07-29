#include "FGpch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGlTextures.h"


namespace Forge {
	Ref<Texture2D> Texture2D::Create(const std::string& filepath) {

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		FG_CORE_ASSERT(false, "Currently does not support RendererAPI::None"); return nullptr;
			case RendererAPI::API::OpenGL:		return std::make_shared<OpenGlTexture2D>(filepath);
		}

		return nullptr;
	}
}