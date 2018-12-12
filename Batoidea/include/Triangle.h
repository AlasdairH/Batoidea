#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "Logger.h"
#include "Ray.h"
#include "Intersect.h"

constexpr float kEpsilon = 1e-8;

namespace Batoidea
{
	/*! @class Triangle
	*	@brief A triangle with 3 vertices and 3 normals
	*
	*	A triangle which contains 3 vertices and 3 normals. Can be tested for intersection with a ray.
	*/
	class Triangle
	{
	public:
		/** @brief Triangle Constructor
		*
		*	Creates a default triangle.
		*/
		Triangle();
		/** @brief Triangle Constructor
		*	@param _v0 The first vertex
		*	@param _v1 The second vertex
		*	@param _v2 The third vertex
		*
		*	Creates a triangle with the specified vertices
		*/
		Triangle(const glm::vec3 &_v0, const glm::vec3 &_v1, const glm::vec3 &_v2);

		/** @brief Triangle Constructor
		*	@param _ray The ray to test against
		*	@param _limits The limits of the intersection, any intersect must be in-between these.
		*	@return The intersect between the ray and the triangle
		*
		*	Tests the triangle for an intersection with the ray, in-between the limits
		*/
		Intersect intersect(const Ray &_ray, const Intersect &_limits);

		glm::vec3 verts[3];		/**< The 3 vertices of the triangle */
		glm::vec3 normals[3];	/**< The 3 normals for each vertex of the triangle */
		glm::vec2 texture[3];	/**< The 3 normals for each vertex of the triangle */
	};
}
