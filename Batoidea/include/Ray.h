#pragma once

// cstdlib

// external libs
#include "GLM/common.hpp"

// program

namespace Batoidea
{
	struct Ray
	{
		Ray();
		Ray(glm::vec3 _origin, glm::vec3 _direction);

		inline glm::vec3 point_at(const float _t) const { return origin + _t * direction; }

		glm::vec3 origin;
		glm::vec3 direction;

		// ray -> aabb pre-optimisation
		glm::vec3 inverseDirection;
		int sign[3];
	};
}