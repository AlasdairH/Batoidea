#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "Logger.h"
#include "Ray.h"
#include "Renderable.h"
#include "Intersect.h"

constexpr float kEpsilon = 1e-8;

namespace Batoidea
{
	class Triangle : public Renderable
	{
	public:
		Triangle(const glm::vec3 &_v0, const glm::vec3 &_v1, const glm::vec3 &_v2);

		Intersect intersect(const Ray &_ray) override;

	protected:
		glm::vec3 m_v0;
		glm::vec3 m_v1;
		glm::vec3 m_v2;
	};
}
