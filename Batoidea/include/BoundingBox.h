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
	struct BoundingBox
	{
		BoundingBox() { bounds[0] = glm::vec3(INFINITY); bounds[1] = glm::vec3(-INFINITY); }

		glm::vec3 bounds[2];

		bool intersect(const Ray &_ray);
	};

}