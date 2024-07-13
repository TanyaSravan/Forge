#pragma once
#include "FGpch.h"
#include "Core.h"
#include "Event/Event.h"


namespace Forge {

	struct WindowProp {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProp(std::string title = "Forge Engine", unsigned int width = 1280, unsigned int height = 720):
		Title(title), Width(width), Height(height) {}

	};

	class FORGE_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;
		virtual inline unsigned int GetWidth() const = 0;
		virtual inline unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		inline virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProp prop = WindowProp());
	};
}
  