#pragma once

// cstdlib

// external libs
#include "GLM/common.hpp"

// program

namespace Batoidea
{
	struct Ray
	{
		Ray() 
		{
			origin = glm::vec3(0);
			direction = glm::vec3(0);
		}

		Ray(glm::vec3 _origin, glm::vec3 _direction) 
		{
			origin = _origin; 
			direction = _direction;
		}

		inline glm::vec3 point_at(const float _t) const { return origin + _t * direction; }

		glm::vec3 origin;
		glm::vec3 direction;
	};
}