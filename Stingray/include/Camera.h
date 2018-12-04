#pragma once

// cstdlib

// external libs
#include "GLM/glm.hpp"
#include "GLM/common.hpp"
#include "GLM/geometric.hpp"

// program
#include "Logger.h"
#include "Ray.h"

namespace Batoidea
{
	class Camera
	{
	public:
		Camera(const int _viewportResolutionWidth, const int _viewportResolutionHeight, glm::vec3 _position);

		Ray getRay(const int _x, const int _y);

	protected:
		int m_viewportResolutionWidth;
		int m_viewportResolutionHeight;

		glm::mat4 m_projectionMatrix;

		glm::mat4 m_viewMatrix;

		glm::vec3 m_position;
		glm::vec3 m_lookAt;
		glm::vec3 m_viewDirection;

		float m_fieldOfView; // radians
		float m_halfFieldOfView; // precompute of the FOV division
		float m_aspectRatio;

		glm::vec3 m_U;
		glm::vec3 m_V;

		glm::vec3 m_viewPlaneBottomLeftPoint;
		glm::vec3 m_xIncVector;
		glm::vec3 m_yIncVector;
	};
}
