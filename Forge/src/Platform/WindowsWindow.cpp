#include "FGpch.h"
#include "WindowsWindow.h"


namespace Forge {

	static bool s_GLFWInitialised = false;

	Window* Window::Create(const WindowProp prop) {
		return new WindowsWindow(prop);
	}

	WindowsWindow::WindowsWindow(const WindowProp& prop) {
		Init(prop);
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProp& prop) {
		m_data.Title = prop.Title;
		m_data.Width = prop.Width;
		m_data.Height = prop.Height;

		FG_CORE_INFO("New Window is being Created {0} (Width, Height): ({1},{2})",
			prop.Title, prop.Width, prop.Height);

		if (!s_GLFWInitialised) {
			int success = glfwInit();
			FG_CORE_ASSERT(success, "GLFW Initialisation failed");

			s_GLFWInitialised = true;
		}

		m_window = glfwCreateWindow((int)prop.Width, (int)prop.Height, m_data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &m_data);
		SetVSync(true);

	}

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(m_window);
	}

	void WindowsWindow:: OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void WindowsWindow::SetVSync(bool enabled) {
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const {
		return m_data.VSync;
	}

}