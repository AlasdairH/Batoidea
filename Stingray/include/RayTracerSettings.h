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
		int resolution_width = 800;
		int resolution_height = 600;

		int threads = 1;

		float fov = 30.0f;

		int maxRayDepth = 5;

		glm::vec3 cameraPosition;
	};
}
