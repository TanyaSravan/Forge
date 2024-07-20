#include "FGpch.h"
#include "RenderCommands.h"
#include "Platform/OpenGL/OpenGlRendererAPI.h"

namespace Forge {
	RendererAPI* RenderCommands::s_RendererAPI = new OpenGlRendererAPI;
}