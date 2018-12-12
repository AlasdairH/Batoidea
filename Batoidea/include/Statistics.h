#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

namespace Batoidea
{
	struct Statistics
	{
		/** @brief Statistics Constructor
		*
		*	Creates and resets a set of stats
		*/
		Statistics() { reset(); }

		/** @brief Resets the stats to 0
		*
		*	Resets all stats to their default values
		*/
		void reset()
		{
			numTriangles = 0;
			numIntersectionTests = 0;
			numIntersections = 0;
		}

		/** @brief Prints out the stats to the console
		*
		*	Prints out the stats to the console
		*/
		void print()
		{
			LOG_MESSAGE("Number of Triangles: " << numTriangles);
			LOG_MESSAGE("Number of Intersection Tests: " << numIntersectionTests);
			LOG_MESSAGE("Number of Intersections: " << numIntersections);
			LOG_MESSAGE("Shadow Rays Cast: " << numShadowRays);
		}

		int numTriangles;			/**< The number of triangles in the scene */
		int	numIntersectionTests;	/**< The number of intersection tests performed */
		int numIntersections;		/**< The number of intersections that tested true */
		int numShadowRays;			/**< The number of shadow rays caculated */
	};
}