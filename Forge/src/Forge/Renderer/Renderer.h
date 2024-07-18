#pragma once

namespace Forge {

	enum class RenderAPI
	{
		None = 0,
		OpenGL = 1,
	};

	class Renderer {
	public:
		inline static RenderAPI GetAPI() { return s_RenderAPI; }

	private:
		static RenderAPI s_RenderAPI;
	};
}
