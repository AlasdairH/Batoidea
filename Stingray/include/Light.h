#pragma once

// cstdlib

// external libs
#include "GLM/common.hpp"

// program
#include "Logger.h"

namespace Batoidea
{
	enum LightType { LIGHT_POINT, LIGHT_DIRECTIONAL };

	struct Light
	{
	public:
		Light() { }
		Light(glm::vec3 _pos, float _int)
		{
			position = _pos;
			intensity = _int;
		}

		LightType type = LIGHT_POINT;

		glm::vec3 position;
		glm::vec3 direction;
		float intensity;
	};
}