#include "FGpch.h"
#include "CameraController.h"
#include "Forge/Core/Input.h"
#include "Forge/Core/Keycodes.h"
#include "Forge/Core/Core.h"

namespace Forge {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:m_aspectRatio(aspectRatio), 
		m_Camera(-m_aspectRatio* m_ZoomLevel, m_aspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), 
		m_rotation(rotation),
		m_Bounds({ -m_aspectRatio * m_ZoomLevel, m_aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }) {}

	void OrthographicCameraController::OnUpdate(Timestep time) {
		FG_PROFILE_FUNCTION();
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
		FG_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(FG_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolledEvent));
		dispatcher.Dispatch<WindowResizeEvent>(FG_BIND_EVENT_FN(OrthographicCameraController::OnWindowResisedEvent));
	}

	void OrthographicCameraController::CalculateView()
	{
		m_Camera.SetCamProjection(-m_aspectRatio * m_ZoomLevel, m_aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		m_Bounds.left = -m_aspectRatio * m_ZoomLevel;
		m_Bounds.right = m_aspectRatio * m_ZoomLevel;
		m_Bounds.bottom = -m_ZoomLevel;
		m_Bounds.top = m_ZoomLevel;
	}

	bool OrthographicCameraController::OnMouseScrolledEvent(MouseScrolledEvent& e) {
		FG_PROFILE_FUNCTION();
		m_ZoomLevel -= e.GetYOffset()* 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

		CalculateView();

		return false;
	}

	bool OrthographicCameraController::OnWindowResisedEvent(WindowResizeEvent& e) {
		FG_PROFILE_FUNCTION();
		m_aspectRatio = (float)e.GetWindowWidth() / (float)e.GetWindowHeight();
		
		CalculateView();
		return false;
	}



}