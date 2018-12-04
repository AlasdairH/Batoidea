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
		glm::vec3 position;
		float intensity;
	};
}