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

		int renderQuadResolutionWidth = 7;
		int renderQuadResolutionHeight = 7;

		float fov = 30.0f;

		int reflectionRecursionDepth = 1;

		float ambientLight = 0.05f;
		glm::vec3 backgroundColour = glm::vec3(0.1f, 0.1f, 0.1f);

		glm::vec3 cameraPosition;
	};
}