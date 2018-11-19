#pragma once

// cstdlib
#include <algorithm>

// external libs
#include "GLM/common.hpp"
#include "GLM/geometric.hpp"

// program
#include "Logger.h"
#include "Ray.h"

namespace Batoidea
{
	class Sphere
	{
	public:
		glm::vec3 centre;
		float radius;

		Sphere(const glm::vec3 &_centre, const float &_radius)
		{
			centre = _centre;
			radius = _radius;
		}

		float intersect(const Ray &_ray);

	protected:
		glm::vec3 m_centre;
		float m_radius;
		
	};
}
