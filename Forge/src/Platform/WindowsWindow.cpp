#include "FGpch.h"
#include "WindowsWindow.h"
#include "Forge/Event/ApplicationEvents.h"
#include "Forge/Event/KeyEvents.h"
#include "Forge/Event/MouseEvents.h"
#include "glad/glad.h"


namespace Forge {

	static bool s_GLFWInitialised = false;

	static void GLFWErrorCallback(int error_code, const char* description) {
		FG_CORE_ERROR("GLFE Error -> ({0}): {1}", error_code, description);
	}

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
			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialised = true;
		}

		m_window = glfwCreateWindow((int)prop.Width, (int)prop.Height, m_data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_window);

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		FG_ASSERT(status, "GLAD failed to initalize");

		glfwSetWindowUserPointer(m_window, &m_data);
		SetVSync(true);

		//Set up all the GLFW callbacks 
		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent resizeEvent(width, height);
			data.Callback(resizeEvent);
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			WindowCloseEvent closeEvent;
			data.Callback(closeEvent);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS: {
					KeyPressedEvent event(key, 0);
					data.Callback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.Callback(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1);
					data.Callback(event);
					break;
				}
			}

		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
				case GLFW_PRESS: {
					MouseButtonPressedEvent event(button);
					data.Callback(event);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleasedEvent event(button);
					data.Callback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xoffset, (float)yoffset);
			data.Callback(event);
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xpos, (float)ypos);
			data.Callback(event);
		});

		

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