#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

namespace Batoidea
{
	struct Statistics
	{
		int numTriangles			= 0;
		int	numIntersectionTests	= 0;
		int numIntersections		= 0;
		int numShadowRays			= 0;

		void reset()
		{
			numTriangles = 0;
			numIntersectionTests = 0;
			numIntersections = 0;
		}

		void print()
		{
			LOG_MESSAGE("Number of Triangles: " << numTriangles);
			LOG_MESSAGE("Number of Intersection Tests: " << numIntersectionTests);
			LOG_MESSAGE("Number of Intersections: " << numIntersections);
			LOG_MESSAGE("Shadow Rays Cast: " << numShadowRays);
		}
	};
}