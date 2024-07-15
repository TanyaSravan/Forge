#pragma once
#include "Core.h"
#include "Event/Event.h"

namespace Forge {
	
	class FORGE_API Layer {
	public:
		Layer(const std::string& name = "Layer");

		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetLayerName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}


