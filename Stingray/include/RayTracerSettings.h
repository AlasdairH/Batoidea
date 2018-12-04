#pragma once

// cstdlib

// external libs
#include "GLM/common.hpp"

// program
#include "Logger.h"

namespace Batoidea
{
	struct RayTracerSettings
	{
		int renderResolutionWidth = SCREEN_WIDTH;
		int renderResolutionHeight = SCREEN_HEIGHT;

		int threads = 1;

		float fov = 30.0f;

		int maxRayDepth = 5;

		float ambientLight = 0.05f;
		glm::vec3 backgroundColour = glm::vec3(0.1f, 0.1f, 0.1f);

		glm::vec3 cameraPosition;
	};
}
