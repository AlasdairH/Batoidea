#pragma once

// cstdlib

// external libs
#include "GLM/common.hpp"

// program

namespace Stingray
{
	struct Ray
	{
		glm::vec3 origin;
		glm::vec3 direction;
		inline glm::vec3 point_at(const float _t) { return origin + _t * direction; }
	};
}