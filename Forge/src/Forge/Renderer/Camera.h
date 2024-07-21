#pragma once
#include "glm/glm.hpp"

namespace Forge {
	class OrthographicCamera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		glm::mat4 GetViewMatrix() { return m_ViewMatrix; }
		glm::mat4 GetProjectionMatrix() { return m_ProjectionMatrix; }
		glm::mat4 GetVPMatrix() { return m_VPMatrix; }

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
		void SetRotation(const float& rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

	private:
		void RecalculateViewMatrix();

	private:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_VPMatrix;

		glm::vec3 m_Position;
		float m_Rotation; 
	};
}
