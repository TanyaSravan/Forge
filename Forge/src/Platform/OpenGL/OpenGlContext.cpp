#include "FGpch.h"
#include "OpenGlContext.h"
#include "Forge/Core.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
namespace Forge {

	OpenGlContext::OpenGlContext(GLFWwindow* windowHandle) 
		:m_windowHandle(windowHandle)
	{
		FG_CORE_ASSERT(m_windowHandle, "m_window does not exist");
	}

	void OpenGlContext::Init() {
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		FG_ASSERT(status, "GLAD failed to initalize");
	}

	void OpenGlContext::SwapBuffers() {
		glfwSwapBuffers(m_windowHandle);
	}
}