#include "Forge.h"

class ExampleLayer : public Forge::Layer {
public:
	ExampleLayer():
		Layer("Example") {}

	void OnUpdate() override {
	}

	void OnEvent(Forge::Event& event) override {
		FG_TRACE("{0}", event);
	}

};


class Sandbox : public Forge::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Forge::ImGuiLayer());

	}

	~Sandbox() {

	}

};

Forge::Application* Forge::CreateApplication() {
	return new Sandbox();
}

