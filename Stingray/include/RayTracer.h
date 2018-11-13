#pragma once

// cstdlib
#include <iostream>
#include <vector>

// external libs
#include "SDL\SDL.h"
#include "GLM/common.hpp"

// program
#include "Logger.h"
#include "Sphere.h"
#include "RayTracerSettings.h"
#include "Ray.h"

namespace Batoidea
{
	class RayTracer
	{
	public:
		RayTracer(const RayTracerSettings _settings);

		SDL_Surface render(std::vector<Sphere> &_renderables, SDL_Surface &_surface);

	protected:
		glm::vec3 trace(const Ray _ray, const std::vector<Sphere> &spheres, const int &depth);

		float mix(const float &a, const float &b, const float &mix)
		{
			return b * mix + a * (1 - mix);
		}

		RayTracerSettings m_settings;
	};
}
