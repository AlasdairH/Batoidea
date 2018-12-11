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

		void addTriange(const Triangle &_tri) { tris.push_back(_tri); }

		inline Material getMaterial() { return m_material; }
		inline void setMaterial(Material _material) { m_material = _material; }

		std::vector<Triangle> tris;
	protected:

		Material m_material;
	};
}