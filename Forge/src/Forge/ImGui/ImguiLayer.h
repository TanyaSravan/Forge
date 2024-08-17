#pragma once
#include "Forge/Core/Core.h"
#include "Forge/Core/Layer.h"
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

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};
}