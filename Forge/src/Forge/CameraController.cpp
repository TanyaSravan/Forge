#include "FGpch.h"
#include "CameraController.h"
#include "Forge/Input.h"
#include "Forge/Keycodes.h"
#include "Forge/Core.h"

namespace Forge {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:m_aspectRatio(aspectRatio), 
		m_Camera(-m_aspectRatio* m_ZoomLevel, m_aspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), 
		m_rotation(rotation) {}

	void OrthographicCameraController::OnUpdate(Timestep time) {
		if (Input::IsKeyPressed(FG_KEY_A))
			m_CamPosition.x += m_CamMoveSpeed * time;
		else if (Input::IsKeyPressed(FG_KEY_D))
			m_CamPosition.x -= m_CamMoveSpeed * time;
		if (Input::IsKeyPressed(FG_KEY_W))
			m_CamPosition.y += m_CamMoveSpeed * time;
		else if (Input::IsKeyPressed(FG_KEY_S))
			m_CamPosition.y -= m_CamMoveSpeed * time;

		if (m_rotation) {

			if (Input::IsKeyPressed(FG_KEY_RIGHT))
				m_CamRotation += m_CamRotSpeed * time;
			else if (Input::IsKeyPressed(FG_KEY_LEFT))
				m_CamRotation -= m_CamRotSpeed * time;

			m_Camera.SetRotation(m_CamRotation);
		}

		m_Camera.SetPosition(m_CamPosition);
		m_CamMoveSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(FG_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolledEvent));
		dispatcher.Dispatch<WindowResizeEvent>(FG_BIND_EVENT_FN(OrthographicCameraController::OnWindowResisedEvent));
	}

	bool OrthographicCameraController::OnMouseScrolledEvent(MouseScrolledEvent& e) {
		m_ZoomLevel -= e.GetYOffset()* 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

		m_Camera.SetCamProjection(-m_aspectRatio * m_ZoomLevel, m_aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResisedEvent(WindowResizeEvent& e) {
		m_aspectRatio = (float)e.GetWindowWidth() / (float)e.GetWindowHeight();
		m_Camera.SetCamProjection(-m_aspectRatio * m_ZoomLevel, m_aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}



}