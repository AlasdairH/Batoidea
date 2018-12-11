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
		Triangle();
		Triangle(const glm::vec3 &_v0, const glm::vec3 &_v1, const glm::vec3 &_v2);

		Intersect intersect(const Ray &_ray, const Intersect &_limits) override;

		glm::vec3 verts[3];
		glm::vec3 normals[3];
	};
}
