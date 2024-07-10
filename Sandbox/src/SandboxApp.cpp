#include "Forge.h"

class ExampleLayer : public Forge::Layer {
public:
	ExampleLayer():
		Layer("Example") {}

	void OnUpdate() override {
		FG_INFO("This is the Example Layer Updatefunc");
	}

	void OnEvent(Forge::Event& event) override {
		FG_TRACE("{0}", event);
	}

};


class Sandbox : public Forge::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());

	}

	~Sandbox() {

	}

};

Forge::Application* Forge::CreateApplication() {
	return new Sandbox();
}

