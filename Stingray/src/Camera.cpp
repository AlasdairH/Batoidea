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

		m_fieldOfView = glm::radians(50.0f);
		m_aspectRatio = (float)m_viewportResolutionWidth / (float)m_viewportResolutionHeight;
		m_halfFieldOfView = m_fieldOfView / 2.0f;
	}

	Ray Camera::getRay(const int _x, const int _y)
	{
		// normalised device coords, add 0.5 to ensure ray passes through the middle of a pixel
		float pixelNDCX = (_x + 0.5f) / m_viewportResolutionWidth;
		float pixelNDCY = (_y + 0.5f) / m_viewportResolutionHeight;

		// correct pixels from [0:1] to [-1:1]
		float pixelNormalisedX = (2 * pixelNDCX - 1);
		float pixelNormalisedY = 1 - 2 * pixelNDCY;

		// accounting for field of view
		float pixelCameraX = pixelNormalisedX * tan(m_halfFieldOfView) * (m_aspectRatio); // correct for aspect ratio
		float pixelCameraY = pixelNormalisedY * tan(m_halfFieldOfView);

		glm::vec3 pixCameraSpace = glm::vec3(pixelCameraX, pixelCameraY, 1);
		
		return Ray(m_position, glm::normalize(pixCameraSpace - m_position));
	}
}