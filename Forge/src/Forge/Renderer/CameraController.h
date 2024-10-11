#pragma once
#include "Forge/Renderer/Camera.h"
#include "Forge/Core/Timestep.h"

#include "Forge/Event/MouseEvents.h"
#include "Forge/Event/ApplicationEvents.h"

namespace Forge {

	struct OrthographicCameraBounds {
		float left, right, bottom, top;

		float GetWidth() { return right - left; }
		float GetHeight() { return top - bottom; }
	};


	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep time);
		void OnEvent(Event& e);

		OrthographicCamera GetCamera() { return m_Camera; }
		const OrthographicCamera GetCamera() const { return m_Camera; }

		OrthographicCameraBounds GetBounds() { return m_Bounds; }

		void SetZoomLevel(const float& zoom) { m_ZoomLevel = zoom; CalculateView(); }
		void CalculateView();

	private:
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnWindowResisedEvent(WindowResizeEvent& e);

	private:
		float m_aspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_rotation;

		glm::vec3 m_CamPosition = { 0.0f,0.0f,0.0f };
		float m_CamRotation = 0.0f;
		float m_CamMoveSpeed = 5.0f;
		float m_CamRotSpeed = 180.0f;

		OrthographicCameraBounds m_Bounds;
	};
}
