#include "Forge.h"

class Sandbox : public Forge::Application {
public:
	Sandbox() {

	}

	~Sandbox() {

	}

};

Forge::Application* Forge::CreateApplication() {
	return new Sandbox();
}

