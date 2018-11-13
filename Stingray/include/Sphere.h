#pragma once

// cstdlib
#include <algorithm>

// external libs
#include "GLM/common.hpp"
#include "GLM/geometric.hpp"

// program
#include "Renderable.h"

// TODO: New sphere class

namespace Batoidea
{
	class Sphere
	{
	public:
		glm::vec3 centre;
		float radius;
		float radiusSq;
		glm::vec3 surfaceColour;
		glm::vec3 emissionColour;
		float transparency;
		float reflection;

		Sphere(
			const glm::vec3 &c,
			const float &r,
			const glm::vec3 &sc,
			const glm::vec3 &ec,
			const float &refl = 0,
			const float &transp = 0) :
			centre(c), radius(r), radiusSq(r * r), surfaceColour(sc), emissionColour(ec),
			transparency(transp), reflection(refl)
		{

		}

		bool intersect(const glm::vec3 &rayorig, const glm::vec3 &raydir, float &t0, float &t1) const
		{
			glm::vec3 l = centre - rayorig;
			float tca = glm::dot(l, raydir);
			if (tca < 0) return false;
			float d2 = glm::dot(l, l) - tca * tca;
			if (d2 > radiusSq) return false;
			float thc = sqrt(radiusSq - d2);
			t0 = tca - thc;
			t1 = tca + thc;

			return true;
		}

	protected:
		
	};
}
