#pragma once
#include "Forge/Core.h"
#include "Forge/Layer.h"
#include "Forge/Event/MouseEvents.h"
#include "Forge/Event/ApplicationEvents.h"
#include "Forge/Event/KeyEvents.h"

namespace Forge {
	class FORGE_API ImGuiLayer : public Layer {

	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;

	private:
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnKeyReleased(KeyReleasedEvent& e);
		bool OnKeyTyped(KeyTypedEvent& e);
		bool OnWindowResised(WindowResizeEvent& e);

	private:
		float m_Time = 0.0f;
	};
}