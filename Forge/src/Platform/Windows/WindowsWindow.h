#pragma once
#include "Forge/Window.h"
#include "GLFW/glfw3.h"
#include "Forge/Renderer/GraphicsContext.h"


namespace Forge {

	class FORGE_API WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProp& prop);

		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_data.Width; }
		inline unsigned int GetHeight() const override { return m_data.Height; }

		void SetEventCallback(const EventCallbackFn& callback) override { m_data.Callback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const override { return m_window; }

	private:
		virtual void Init(const WindowProp& prop);
		virtual void Shutdown();
	private:
		GLFWwindow* m_window;
		GraphicsContext* m_Context;

		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFn Callback;
		};

		WindowData m_data;
	};
}