#pragma once

// cstdlib

// external libs
#include "GLM/common.hpp"

// program

namespace Batoidea
{
	struct Intersect
	{
		Intersect()
		{
			t1 = INFINITY;
			t2 = INFINITY;
		}
		Intersect(float _t1, float _t2)
		{
			t1 = _t1;
			t2 = _t2;
		}

		float t1;
		float t2;
	};
}