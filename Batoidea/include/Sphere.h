#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "Logger.h"
#include "Ray.h"
#include "Renderable.h"
#include "Intersect.h"

namespace Batoidea
{
	class Sphere : public Renderable
	{
	public:
		Sphere(const glm::vec3 &_centre, const float &_radius, const Material _material)
		{
			m_centre = _centre;
			m_radius = _radius;
			m_material = _material;
		}

		Intersect intersect(const Ray &_ray, const Intersect &_intersect) override;

		inline glm::vec3 getCentre() { return m_centre; }

	protected:
		glm::vec3 m_centre;
		float m_radius;
	};
}
