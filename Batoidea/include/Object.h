#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "Triangle.h"
#include "Intersect.h"
#include "BoundingBox.h"

namespace Batoidea
{
	class Object
	{
	public:
		Object() { }

		void addTriange(const Triangle &_tri) { tris.push_back(_tri); }

		inline Material getMaterial() { return m_material; }
		inline void setMaterial(Material _material) { m_material = _material; }

		inline bool boundIntersection(const Ray &_ray) { return m_boundingBox.intersect(_ray); }

		inline void setBoundingBox(BoundingBox &_bb) { m_boundingBox = _bb; }

		std::vector<Triangle> tris;
	protected:

		Material m_material;
		BoundingBox m_boundingBox;
	};
}