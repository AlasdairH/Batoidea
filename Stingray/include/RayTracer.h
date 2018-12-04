#pragma once

// cstdlib
#include <iostream>
#include <vector>

// external libs
#include "SDL\SDL.h"
#include "GLM/common.hpp"
#include "GLM/glm.hpp"

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

		/** @brief Primary Render Method
		*	@param _renderables The vector of renderbales to render to the surface
		*	@param _lights The vector of lights to render with
		*	@param _surface The SDL surface to render the image to
		*	@return The rendered SDL surface
		*
		*	Takes renderables and a renderable surface and ray traces the scene from the camera
		*/
		SDL_Surface render(std::vector<Sphere> &_renderables, std::vector<Light> &_lights, SDL_Surface &_surface);

	protected:
		/** @brief Per pixel trace method
		*	@param _ray The ray to trace through the scene
		*	@param _objects The list of objects that can be rendered
		*	@param _lights The lights to send to the light compute method
		*	@return The colour (normalised) at the ray end
		*
		*	The per pixel render method which traces a ray through the scene.
		*/
		glm::vec3 trace(const Ray &_ray, std::vector<Sphere> &_objects, std::vector<Light> &_lights);
		/** @brief Lighting Calculation 
		*	@param _normal The normal of the point to calculate lighting on
		*	@param _lights The lights to compute with
		*	@param _position The position in space of which to calculate the lighting of
		*	@return The light intensity at the position
		*
		*	Computes the lighting for a given position and normal
		*/
		float computeLighting(std::vector<Light> &_lights, glm::vec3 _normal, glm::vec3 _position);

		/** @brief Sets a surface pixel colour
		*	@param _surface The number of worker threads to create
		*	@param _x The x position on the surface
		*	@param _y The y position on the surface
		*	@param _r The red component
		*	@param _g The green component
		*	@param _b The blue component
		*
		*	Creates a thread pool with the specified number of threads
		*/
		void setPixelColour(SDL_Surface &_surface, const int _x, const int _y, const int _r, const int _g, const int _b);

		RayTracerSettings m_settings;	/**< The ray tracer settings */

		//std::vector<Sphere

		float mix(const float &a, const float &b, const float &mix)
		{
			return b * mix + a * (1 - mix);
		}
	};
}
