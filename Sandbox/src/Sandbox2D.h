#pragma once
#include "Forge.h"

class Sandbox2D : public Forge::Layer {
public:
	Sandbox2D();
	~Sandbox2D() {}

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Forge::Timestep time) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Forge::Event& event) override;

private:
	Forge::Ref<Forge::VertexArray> m_SquareVA;
	Forge::Ref<Forge::Shader> m_BlueShader;

	Forge::Ref<Forge::Texture2D> m_Texture2D;

	glm::vec3 m_SquareColor = { 0.2f,0.6f,0.4f };

	Forge::OrthographicCameraController m_orthoCamController;

	struct ProfileData {
		const char* name;
		float time;
	};

	std::vector<ProfileData> m_Profile;

};