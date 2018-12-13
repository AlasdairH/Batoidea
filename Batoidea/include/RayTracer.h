#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "Logger.h"
#include "RenderQuad.h"
#include "RayTracerSettings.h"
#include "Ray.h"
#include "Timer.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "Statistics.h"

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
		SDL_Surface render(std::vector<Object> &_renderables, std::vector<Light> &_lights, SDL_Surface &_surface);

		/** @brief Calculates the render quads
		*	@return The list of RenderQuads for the viewport
		*
		*	Takes the viewport and divides it up into smaller segments called RenderQuads which each represent a segment of the final render. These
		*	RenderQuads are then added to the threadpool to be ray traced.
		*/
		bool isRenderComplete();

		/** @brief Stops any active render 
		*
		*	Checks for an active render, if there is one then stop it
		*/
		inline void stopCurrentRender() { m_threadPool->cancelQueue(); }

		/** @brief Returns the current or last render statistics
		*	@return The current statistics for the render
		*
		*	Returns the render statistics. This might not be complete if a render is not finished.
		*/
		inline Statistics getRenderStatistics() { return m_statistics; }

	protected:
		/** @brief Returns the closest intersection for a given ray
		*	@param _renderable The closest renderable object
		*	@param _closestIntersection The distance of the closest intersection
		*	@param _normal The normal at the intersection
		*	@param _ray The ray to cast to find any intersections
		*	@param _limits The limits to find any intersection inbetween
		*	@return The calculated intersection
		*
		*	Takes the renderbale, intersection distance and normal by reference to return multiple values. Tests the scene for intersections
		*	with the ray and returns the results.
		*/
		Intersect calculateClosestIntersection(std::shared_ptr<Object> &_renderable, Ray _ray, Intersect _limits);

		/** @brief Calculates the render quads
		*	@return The list of RenderQuads for the viewport
		*
		*	Takes the viewport and divides it up into smaller segments called RenderQuads which each represent a segment of the final render. These
		*	RenderQuads are then added to the threadpool to be ray traced.
		*/
		std::vector<RenderQuad> calculateRenderQuads();

		/** @brief Renders a specified segment of the image
		*	@param _start the top left coordinate of the quad to render
		*	@param _finish the bottom right coordinate of the quad to render
		*
		*	Ray traces a segment of the image. Could be the entire thing or just a small part of it, typically a RenderQuad.
		*/
		void renderQuadToPixels(const glm::vec2 _start, const glm::vec2 _finish);

		/** @brief Per pixel trace method
		*	@param _ray The ray to trace through the scene
		*	@param _depth The reflection depth (if needed)
		*	@param _limits The near and far limits of the ray trace
		*	@return The colour (normalised) at the ray end
		*
		*	The per pixel render method which traces a ray through the scene.
		*/
		glm::vec3 trace(const Ray &_ray, int _depth, Intersect _limits);

		/** @brief Lighting Calculation 
		*	@param _renderable The object that's having it's lighting computed
		*	@param _normal The normal of the point to calculate lighting on
		*	@param _position The position in space of which to calculate the lighting of
		*	@param _rayToCamera a vector from the object to the camaera ( - _ray.direction )
		*	@return The light intensity at the position
		*
		*	Computes the lighting for a given position and normal
		*/
		float computeLighting(std::shared_ptr<Object> _renderable, glm::vec3 _normal, glm::vec3 _position, glm::vec3 _rayToCamera);

		/** @brief Reflects a ray by a normal
		*	@param _rayDir The direction of the ray to reflect
		*	@param _normal The normal to relfect the ray by
		*	@return The reflected direction
		*
		*	Takes a ray direction and reflects it based on the given normal.
		*/
		glm::vec3 reflect(glm::vec3 _rayDir, glm::vec3 _normal) 
		{
			return 2.0f * _normal * glm::dot(_normal, _rayDir) - _rayDir;
		}

		Timer										m_timer;			/**< A timer for timing ray tracing operations */

		std::shared_ptr<Threads::ThreadPool>		m_threadPool;		/**< The thread pool used to render the scene */

		std::shared_ptr<Camera>						m_camera;			/**< The scene camera */
		std::vector<Object>							m_objects;			/**< Vector of objects in the scene */
		std::vector<Light>							m_lights;			/**< Vector of lights in the scene */

		Uint32										*m_pixels;			/**< Pointer to the pixels of the surface */

		std::mutex									m_statisticMutex;
		RayTracerSettings							m_settings;			/**< The ray tracer settings */
		Statistics									m_statistics;		/**< The stats for the last render */
	};
}
