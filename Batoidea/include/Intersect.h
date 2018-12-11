#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

namespace Batoidea
{
	struct Intersect
	{
		Intersect()
		{
			t1 = INFINITY;
			t2 = INFINITY;
		}

		Intersect(float _t1, float _t2, const glm::vec3 &_normal = glm::vec3(0))
		{
			t1 = _t1;
			t2 = _t2;
			normal = _normal;
		}

		float t1;
		float t2;

		glm::vec3 normal;
	};
}