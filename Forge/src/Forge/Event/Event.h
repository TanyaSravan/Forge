#pragma once

#include "Forge/Core.h"


namespace Forge {

	enum class EventType {
		None = 0,
		//Application Events
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,

		//Keyboard Events
		KeyPressed, KeyReleased,

		//Mouse Events
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4)

	};

#define EVENT_TYPE(type) static EventType GetStaticType(){return EventType::##type;}\
						 virtual EventType GetEventType() const override {return GetStaticType();}\
						 virtual const char* GetName() const override {return #type;}

#define EVENT_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}


	class FORGE_API Event {
	public:
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual EventType GetEventType() const = 0;

		//Debug only function
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}

		bool handled = false;
	};

	class FORGE_API EventDispatcher {
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event):
		m_Event(event) {}

		template<typename T>
		bool Dispatch(EventFn<T> func) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::string format_as(const Event& e) {
		return e.ToString();
	}
}



