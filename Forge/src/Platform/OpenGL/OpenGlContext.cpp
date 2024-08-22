#include "FGpch.h"
#include "OpenGlContext.h"
#include "Forge/Core/Core.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"
namespace Forge {

	OpenGlContext::OpenGlContext(GLFWwindow* windowHandle) 
		:m_windowHandle(windowHandle)
	{
		FG_CORE_ASSERT(m_windowHandle, "m_window does not exist");
	}

	void OpenGlContext::Init() {
		FG_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		FG_ASSERT(status, "GLAD failed to initalize");

		FG_CORE_INFO("OpenGl Renderer:");

		FG_CORE_INFO("    Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		FG_CORE_INFO("    Renderer: {0}", (const char*) glGetString(GL_RENDERER));
		FG_CORE_INFO("    Version: {0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGlContext::SwapBuffers() {
		FG_PROFILE_FUNCTION();
		glfwSwapBuffers(m_windowHandle);
	}
}