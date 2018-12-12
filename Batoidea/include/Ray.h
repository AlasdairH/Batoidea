#pragma once

// cstdlib

// external libs
#include "GLM/common.hpp"

// program

namespace Batoidea
{
	/*! @class Ray
	*	@brief A ray in 3D space
	*
	*	The ray is an infinite ray from point origin in any direction.
	*/
	struct Ray
	{
		/** @brief Ray Constructor
		*
		*	Creates a default ray
		*/
		Ray();
		/** @brief Ray Constructor
		*	@param _origin The origin of the ray
		*	@param _direction The direction of the ray
		*
		*	Creates a ray with the specified arguments
		*/
		Ray(glm::vec3 _origin, glm::vec3 _direction);

		/** @brief Gets a point along the ray
		*	@param _t How far along the ray (forward or backward) to get the point from
		*	@return A point in 3D space along the ray at distance _t
		*
		*	Gets the point in 3D space along the ray at distance _t.
		*/
		inline glm::vec3 point_at(const float _t) const { return origin + _t * direction; }

		glm::vec3 origin;				/**< The origin of the ray */
		glm::vec3 direction;			/**< The direction of the ray */

		// ray -> aabb pre-optimisation
		glm::vec3 inverseDirection;		/**< The inverse direction, precalculated for intersection tests */
		int sign[3];					/**< The positive / negative state of each ray direction value, precalculated for intersection tests */
	};
}