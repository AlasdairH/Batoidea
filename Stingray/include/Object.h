#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "Triangle.h"
#include "Intersect.h"

namespace Batoidea
{
	class Object
	{
	public:
		Object() { }

		void addTriange(const Triangle &_tri) { m_tris.push_back(_tri); }

		Intersect intersect(const Ray &_ray, Intersect &_limits);

		inline Material getMaterial() { return m_material; }
		inline void setMaterial(Material _material) { m_material = _material; }

	protected:
		std::vector<Triangle> m_tris;

		Material m_material;
	};
}