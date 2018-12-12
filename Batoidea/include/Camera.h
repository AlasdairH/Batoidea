#pragma once

// cstdlib

// external libs
#include "GLM/gtx/transform.hpp"

// program
#include "PCH.h"
#include "Logger.h"
#include "Ray.h"

namespace Batoidea
{
	class Camera
	{
	public:
		Camera(const int _viewportResolutionWidth, const int _viewportResolutionHeight, glm::vec3 _position);

		Ray getRay(const float _x, const float _y);

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

		glm::mat4 m_cameraToWorld;
	};
}
