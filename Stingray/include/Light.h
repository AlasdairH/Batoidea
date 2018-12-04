#pragma once

// cstdlib

// external libs
#include "GLM/common.hpp"

// program
#include "Logger.h"

namespace Batoidea
{
	struct Light
	{
	public:
		Light(glm::vec3 _pos, float _int)
		{
			position = _pos;
			intensity = _int;
		}

		glm::vec3 position;
		float intensity;
	};
}