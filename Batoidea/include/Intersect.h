#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

namespace Batoidea
{
	/*! @class Intersect
	*	@brief An intersect with a closest and furthest point as well as a calculated normal for the point of intersection
	*
	*	When a ray intersects an object in the scene, this intersect is generated with the details of the intersect distance and the normal at
	*	the point of intersection.
	*/
	struct Intersect
	{
		/** @brief Intersect Constructor
		*
		*	Sets both intersect points to an infinitely far distance
		*/
		Intersect()
		{
			t1 = INFINITY;
			t2 = INFINITY;
		}

		/** @brief Intersect Constructor
		*	@param _t1 the closest point of intersection
		*	@param _t2 the farthest point of intersection
		*	@param _normal the normal at the point of intersection
		*
		*	Creates an intersection with the provided arguments
		*/
		Intersect(float _t1, float _t2, const glm::vec3 &_normal = glm::vec3(0), const glm::vec2 &_texture = glm::vec2(0))
		{
			t1 = _t1;
			t2 = _t2;
			normal = _normal;
			texCoord = _texture;
		}

		float t1;				/**< The closest point of intersection */
		float t2;				/**< The farthest point of intersection */

		glm::vec3 normal;		/**< The normal at the point of intersection */
		glm::vec2 texCoord;		/**< The texture corrdinate at the point of intersection */
	};
}