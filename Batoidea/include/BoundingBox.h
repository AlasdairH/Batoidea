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
	/*! @class BoundingBox
	*	@brief A data structure representing a bounding box for an object in 3D space
	*
	*	This struct represents a bounding box in 3D space, primarily used for optimising ray tests before checking every 
	*	triangle of the object it represents.
	*/
	struct BoundingBox
	{
		/** @brief BoundingBox Constructor
		*
		*	Creates an infinitely small bounding box
		*/
		BoundingBox() { bounds[0] = glm::vec3(INFINITY); bounds[1] = glm::vec3(-INFINITY); }
		
		/** @brief Bounding Box - Ray intersection test
		*	@param _ray The ray to test against
		*	@return True/False as to if the ray intersects or not
		*
		*	Tests a ray against the bounding box for intersection.
		*/
		bool intersect(const Ray &_ray);
		
		glm::vec3 bounds[2];	/**< The minimum and maximum bounds of the box. [0] = min, [1] = max. */
	};

}