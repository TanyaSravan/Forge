#pragma once

#include "Event.h"

namespace Forge {
	class FORGE_API WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(int width, int height):
		m_Width(width), m_Height(height) {}

		inline int GetWindowWidth() { return m_Width; }
		inline int GetWindowHeight() { return m_Height; }

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_TYPE(WindowResize)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent -> Width: " << m_Width << " Height: " << m_Height;
			return ss.str();
		}

	private:
		int m_Width, m_Height;
	};

	class FORGE_API WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_TYPE(WindowClose)
	};

	class FORGE_API AppTickEvent : public Event {
	public:
		AppTickEvent() {}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_TYPE(AppTick)
	};

	class FORGE_API AppUpdateEvent : public Event {
	public:
		AppUpdateEvent() {}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_TYPE(AppUpdate)
	};

	class FORGE_API AppRenderEvent : public Event {
	public:
		AppRenderEvent() {}

		EVENT_CATEGORY(EventCategoryApplication)
		EVENT_TYPE(AppRender)
	};
}