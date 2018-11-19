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
#include "Timer.h"

namespace Batoidea
{
	class RayTracer
	{
	public:
		RayTracer(const RayTracerSettings _settings);

		SDL_Surface render(std::vector<Sphere> &_renderables, SDL_Surface &_surface);

	protected:
		glm::vec3 trace(const Ray &_ray, std::vector<Sphere> &_objects);

		float mix(const float &a, const float &b, const float &mix)
		{
			return b * mix + a * (1 - mix);
		}

		void setPixelColour(SDL_Surface &_surface, const int _x, const int _y, const int _r, const int _g, const int _b);

		RayTracerSettings m_settings;
	};
}
