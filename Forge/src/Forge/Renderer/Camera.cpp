#include "FGpch.h"
#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Forge {
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) 
		:m_ProjectionMatrix(glm::ortho(left,right,bottom,top,-1.0f,1.0f)), m_ViewMatrix(1.0f), m_Rotation(0.0f), m_Position({0.0f,0.0f,0.0f})
	{
		m_VPMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetCamProjection(float left, float right, float bottom, float top) {
		FG_PROFILE_FUNCTION();
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_VPMatrix = m_ProjectionMatrix * m_ViewMatrix;

	}

	void OrthographicCamera::RecalculateViewMatrix() {
		FG_PROFILE_FUNCTION();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * 
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f,0.0f,1.0f));

		m_ViewMatrix = glm::inverse(transform);
		m_VPMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}