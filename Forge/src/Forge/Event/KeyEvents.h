#pragma once

#include "Event.h"

namespace Forge {

	class FORGE_API KeyEvent : public Event {
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

	protected:
		KeyEvent(int keycode)
			:m_KeyCode(keycode){}

		int m_KeyCode;
	}; 

	class FORGE_API KeyPressedEvent : public KeyEvent {
	public:
		KeyPressedEvent(int keycode, int repeatCount)
		: KeyEvent(keycode), m_repeatCount(repeatCount) {}

		inline int GetRepeatCount() { return m_repeatCount; }

		EVENT_TYPE(KeyPressed)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent -> Keycode: " << m_KeyCode << " Repeat Count: " << m_repeatCount;
			return ss.str();
		}
	private:
		int m_repeatCount;
	};

	class FORGE_API KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keycode)
			:KeyEvent(keycode){}

		EVENT_TYPE(KeyReleased)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent -> Keycode: " << m_KeyCode;
			return ss.str();
		}
	};

	class FORGE_API KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(int keycode)
			:KeyEvent(keycode) {}

		EVENT_TYPE(KeyTyped)

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyTypedEvent -> Keycode: " << m_KeyCode;
			return ss.str();
		}
	};

}
