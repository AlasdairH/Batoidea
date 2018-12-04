#include "Camera.h"

namespace Batoidea
{
	Camera::Camera(const int _viewportResolutionWidth, const int _viewportResolutionHeight, glm::vec3 _position)
	{
		m_position = _position;
		m_viewportResolutionWidth = _viewportResolutionWidth;
		m_viewportResolutionHeight = _viewportResolutionHeight;

		m_lookAt = glm::vec3(0, 0, 0);
		m_viewDirection = m_lookAt - m_position;

		m_U = m_viewDirection * glm::vec3(0, 1, 0);
		m_V = m_U * m_viewDirection;

		m_U = glm::normalize(m_U);
		m_V = glm::normalize(m_V);

		float fieldOfView = 90.0f;

		float viewPlaneHalfWidth = tan(fieldOfView / 2.0f);
		float aspectRatio = m_viewportResolutionHeight / (float)m_viewportResolutionWidth;
		float viewPlaneHalfHeight = aspectRatio * viewPlaneHalfWidth;

		m_viewPlaneBottomLeftPoint = m_lookAt - m_V * viewPlaneHalfHeight - m_U * viewPlaneHalfWidth;
		m_xIncVector = (m_U * 2.0f * viewPlaneHalfWidth) / (float)m_viewportResolutionWidth;
		m_yIncVector = (m_V * 2.0f * viewPlaneHalfHeight) / (float)m_viewportResolutionHeight;
	}

	Ray Camera::getRay(const int _x, const int _y)
	{
		glm::vec3 viewPlanePoint = m_viewPlaneBottomLeftPoint + (float)_x * m_xIncVector + (float)_y * m_yIncVector;
		return Ray(m_position, viewPlanePoint - m_position);
	}
}