#pragma once

#include "Event.h"

namespace Forge {
	class FORGE_API MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float xPos, float yPos): 
			m_Xpos(xPos),m_Ypos(yPos){}

		inline float GetXPos() { return m_Xpos; }
		inline float GetYPos() { return m_Ypos; }

		EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse)
		EVENT_TYPE(MouseMoved)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent -> X Pos: " << m_Xpos << " Y Pos: " << m_Ypos;
			return ss.str();
		}

	private:
		float m_Xpos, m_Ypos;
	};

	class FORGE_API MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float xOffset, float yOffset):
			m_XOffset(xOffset), m_YOffset(yOffset) {}

		inline float GetXOffset() { return m_XOffset; }
		inline float GetYOffset() { return m_YOffset; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrolledEvent -> X Offset: " << m_XOffset << " Y Offset: " << m_YOffset;
			return ss.str();
		}

		EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse)
		EVENT_TYPE(MouseScrolled)

	private:
		float m_XOffset, m_YOffset;
	};

	class FORGE_API MouseButtonEvent :public Event {
	public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse)
	protected:
		MouseButtonEvent(int button) :
			m_Button(button) {}

		int m_Button;
	};

	class FORGE_API MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button) :
			MouseButtonEvent(button) {}

		EVENT_TYPE(MouseButtonPressed)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressedEvent -> Button: " << m_Button;
			return ss.str();
		}

	private:
	};

	class FORGE_API MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button) :
			MouseButtonEvent(button) {}

		EVENT_TYPE(MouseButtonReleased)

			std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent -> Button: " << m_Button;
			return ss.str();
		}

	private:
	};
}
