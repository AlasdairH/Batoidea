#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "Logger.h"

namespace Batoidea
{
	/*! @class RayTracerSettings
	*	@brief The settings the ray tracer will use when rendering
	*
	*	Contains all the settings the ray tracer needs to render the scene. Please pay attention to the reasonable limits.
	*/
	struct RayTracerSettings
	{
		int renderResolutionWidth = SCREEN_WIDTH;						/**< The width of the render */
		int renderResolutionHeight = SCREEN_HEIGHT;						/**< The height of the render */

		int threads = std::thread::hardware_concurrency();				/**< The number of threads to use, by default is set to the number of available threads on the system */

		int renderQuadResolutionWidth = 20;								/**< The number of horizontal segments divide up the render by. The total number of segments should not be less than the number of available threads. */
		int renderQuadResolutionHeight = 20;							/**< The number of vertical segments divide up the render by. The total number of segments should not be less than the number of available threads.*/

		float fov = 30.0f;												/**< The field of view of the camera */

		int reflectionRecursionDepth = 3;								/**< How many reflection recursions to complete (seeing reflections in reflections) */
		int samplesPerPixel = 3;										/**< The number of samples to take per pixel. This will anti-alias the final render a the cost of N * longer renders. */

		glm::vec3 backgroundColour = glm::vec3(0.1f, 0.1f, 0.1f);		/**< The colour to render when no object is intersected */

		glm::vec3 cameraPosition;										/**< The position of the camera in 3D space */
	};
}
