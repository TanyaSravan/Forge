#include "FGpch.h"
#include "Application.h"
#include "Forge/Event/Event.h"
#include "Forge/Event/ApplicationEvents.h"
#include "Forge/Log.h" 

namespace Forge {

	Application::Application(){}

	Application::~Application(){}


	void Application::Run() {
		WindowResizeEvent resiseEvent(1280, 720);
		if (resiseEvent.IsInCategory(EventCategoryApplication)) {
			FG_TRACE(resiseEvent);
		}

		while (true);
	}
}



