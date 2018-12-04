#pragma once

// cstdlib
#include <algorithm>

// external libs
#include "GLM/common.hpp"
#include "GLM/geometric.hpp"

// program
#include "Logger.h"
#include "Ray.h"
#include "Renderable.h"
#include "Intersect.h"

namespace Batoidea
{
	class Sphere : public Renderable
	{
	public:
		Sphere(const glm::vec3 &_centre, const float &_radius, const glm::vec4 _colour)
		{
			m_centre = _centre;
			m_radius = _radius;
			m_colour = _colour;
		}

		Intersect intersect(const Ray &_ray) override;

		inline glm::vec3 getCentre() { return m_centre; }

	protected:
		glm::vec3 m_centre;
		float m_radius;
	};
}
