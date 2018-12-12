#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "Triangle.h"
#include "Intersect.h"
#include "BoundingBox.h"
#include "Material.h"

namespace Batoidea
{
	/*! @class Object
	*	@brief An object in the scene
	*
	*	The object class represents a 3D object comprised of triangles which an overall bounding box.
	*/
	class Object
	{
	public:
		/** @brief RayTracer Constructor
		*
		*	Does nothing
		*/
		Object() { }

		/** @brief Gets the object's material
		*	@return The object's material
		*
		*	Gets the object's material
		*/
		inline Material getMaterial() { return m_material; }
		/** @brief Sets the object's material
		*	@param _material The material to assign to the object
		*
		*	Sets the object's material
		*/
		inline void setMaterial(Material _material) { m_material = _material; }

		/** @brief Tests a ray against the object's bounding box
		*	@param _ray The ray to test
		*	@return Whether the ray intersects the objects bounds
		*
		*	Tests a ray against the boudning box of the object. This is used to optimise the tracing of
		*	the scene by testing to see if the ray intersects the object at all before testing each individual
		*	triangle. While this isn't perfect, it is a huge improvement in render times.
		*/
		inline bool boundIntersection(const Ray &_ray) { return m_boundingBox.intersect(_ray); }

		/** @brief Sets the object's bounding box
		*	@param _bb The boudning box to assign to the object
		*
		*	Sets the bounding box of the object
		*/
		inline void setBoundingBox(BoundingBox &_bb) { m_boundingBox = _bb; }

		std::vector<Triangle> tris;		/**< The triangles that make up the object. (Make this protected?) */
	protected:

		Material m_material;			/**< The material that will be used to render the object */
		BoundingBox m_boundingBox;		/**< The bounding box that surrounds the object */
	};
}