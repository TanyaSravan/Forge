#include "Forge.h"
#include "imgui/imgui.h"


class ExampleLayer : public Forge::Layer {
public:
	ExampleLayer():
		Layer("Example") {}

	void OnUpdate() override {
		if (Forge::Input::IsKeyPressed(FG_KEY_TAB))
			FG_TRACE("Tab key is pressed");
	}

	void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(Forge::Event& event) override {
		if (event.GetEventType() == Forge::EventType::KeyPressed) {
			Forge::KeyPressedEvent& e = (Forge::KeyPressedEvent&)event;
			if (e.GetKeyCode() == FG_KEY_TAB)
				FG_TRACE("Tab Key Was pressed (Event)"); 
			FG_TRACE("{0}", (char)e.GetKeyCode());

		}
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

