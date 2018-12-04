#include "Camera.h"

namespace Batoidea
{
	Camera::Camera(const int _viewportResolutionWidth, const int _viewportResolutionHeight, glm::vec3 _position)
	{
		m_position = _position;
		m_viewportResolutionWidth = _viewportResolutionWidth;
		m_viewportResolutionHeight = _viewportResolutionHeight;
	}

	Ray Camera::getRay(const int _x, const int _y)
	{
		float viewportSize = 1;
		return Ray(m_position, glm::vec3(_x * viewportSize / m_viewportResolutionWidth, _y * viewportSize / m_viewportResolutionHeight, 1));
	}
}