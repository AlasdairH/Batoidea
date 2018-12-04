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
#include "Camera.h"
#include "Light.h"

namespace Batoidea
{
	/*! @class RayTracer
	*	@brief The primary ray tracer class which handles the rendering of a scene
	*
	*	This class is the primary renderer for a list of objects in a scene. The ray tracer holds a thread pool which it uses to distribute
	*	the rendering of the scene over various threads.
	*/
	class RayTracer
	{
	public:
		/** @brief RayTracer Constructor
		*	@param _settings The settings to use when rendering
		*
		*	Creates the raytracer and applies the settings
		*/
		RayTracer(const RayTracerSettings _settings);

		/** @brief ThreadPool Constructor
		*	@param _numThreads The number of worker threads to create
		*
		*	Creates a thread pool with the specified number of threads
		*/
		SDL_Surface render(std::vector<Sphere> &_renderables, SDL_Surface &_surface);

	protected:
		glm::vec3 trace(const Ray &_ray, std::vector<Sphere> &_objects);
		glm::vec3 computeLighting(glm::vec3 _normal, glm::vec3 _position);

		float mix(const float &a, const float &b, const float &mix)
		{
			return b * mix + a * (1 - mix);
		}

		void setPixelColour(SDL_Surface &_surface, const int _x, const int _y, const int _r, const int _g, const int _b);

		RayTracerSettings m_settings;
	};
}
