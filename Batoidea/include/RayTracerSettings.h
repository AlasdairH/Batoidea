#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "Logger.h"

namespace Batoidea
{
	struct RayTracerSettings
	{
		int renderResolutionWidth = SCREEN_WIDTH;
		int renderResolutionHeight = SCREEN_HEIGHT;

		int threads = std::thread::hardware_concurrency();

		int renderQuadResolutionWidth = 20;
		int renderQuadResolutionHeight = 20;

		float fov = 30.0f;

		int reflectionRecursionDepth = 3;
		int samplesPerPixel = 3;

		glm::vec3 backgroundColour = glm::vec3(0.1f, 0.1f, 0.1f);

		glm::vec3 cameraPosition;
	};
}
