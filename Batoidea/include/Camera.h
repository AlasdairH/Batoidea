#pragma once

// cstdlib

// external libs
#include "GLM/gtx/transform.hpp"

// program
#include "PCH.h"
#include "Logger.h"
#include "Ray.h"

namespace Batoidea
{
	/*! @class Camera
	*	@brief A camera in 3D space
	*
	*	This class represents a camera in 3D space which is used to render the scene. The primary method of this class is
	*	the getRay method which is used to convert the x and y pixel to be rendered into a ray.
	*/
	class Camera
	{
	public:
		/** @brief Camera Constructor
		*	@param _viewportResolutionWidth The width of the viewport
		*	@param _viewportResolutionHeight The height of the viewport
		*	@param _position The position to render from
		*
		*	Creates a camera and does any pre-computation possible.
		*/
		Camera(const int _viewportResolutionWidth, const int _viewportResolutionHeight, glm::vec3 _position);

		/** @brief Camera Constructor
		*	@param _x The x pixel to pass through
		*	@param _y The y pixel to pass through
		*	@return A ray going from the camera through pixel _x, _y
		*
		*	Claculates the ray which starts from the camera position and passes through point (x, y)
		*/
		Ray getRay(const float _x, const float _y);

	protected:
		int m_viewportResolutionWidth;		/**< The viewport width in pixels */
		int m_viewportResolutionHeight;		/**< The viewport height in pixels */

		glm::vec3 m_position;				/**< The camera position */

		float m_fieldOfView;				/**< The field of view in Radians */
		float m_halfFieldOfView;			/**< The calculated half FoV */
		float m_aspectRatio;				/**< The calculated aspect ratio */
	};
}
