#pragma once

// cstdlib

// external libs

// program
#include "Intersect.h"


namespace Batoidea
{
	class Renderable
	{
	public:
		Renderable() { }

		virtual Intersect intersect(const Ray &_ray) { };

		inline glm::vec4 getColour() { return m_colour; }
	protected:
		glm::vec4 m_colour;
	};
}