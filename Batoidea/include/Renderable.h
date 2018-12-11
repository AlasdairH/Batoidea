#pragma once

// cstdlib

// external libs

// program
#include "Intersect.h"
#include "Material.h"

namespace Batoidea
{
	class Renderable
	{
	public:
		Renderable() { }

		virtual Intersect intersect(const Ray &_ray, const Intersect &_limits) { return Intersect(INFINITY, INFINITY, glm::vec3(0)); };

		inline Material getMaterial() { return m_material; }

	protected:
		Material m_material;
	};
}