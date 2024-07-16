#pragma once
#include "Forge/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Forge {
	class OpenGlContext :public GraphicsContext {
	public:
		OpenGlContext(GLFWwindow* windowHandle);

		virtual void Init();
		virtual void SwapBuffers();

	private:
		GLFWwindow* m_windowHandle;
	};
}