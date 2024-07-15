#pragma once

#include "Forge/Core.h"
#include "Layer.h"
#include <vector>

namespace Forge {

	class FORGE_API LayerStack {
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*> ::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*> ::iterator end() { return m_layers.end(); }
	private:
		std::vector<Layer*> m_layers;
		unsigned int m_layerInsertIndex = 0;
	};
}
