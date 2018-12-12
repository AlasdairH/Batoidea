#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "RayTracerSettings.h"

namespace Batoidea
{
	/*! @class Statistics
	*	@brief Holds statistics for a ray trace
	*
	*	The statistics are updated by the renderer as it traces the scene and can be used for profiling.
	*/
	class Statistics
	{
	public:
		friend class RayTracer;
		/** @brief Statistics Constructor
		*
		*	Creates and resets a set of stats
		*/
		Statistics() { reset(); }

		/** @brief Resets the stats to 0
		*
		*	Resets all stats to their default values
		*/
		void reset();

		/** @brief Prints out the stats to the console
		*
		*	Prints out the stats to the console
		*/
		void print();

		void outputToCSV();

	protected:
		int numTriangles;			/**< The number of triangles in the scene */
		int	numIntersectionTests;	/**< The number of intersection tests performed */
		int numIntersections;		/**< The number of intersections that tested true */
		int numShadowRays;			/**< The number of shadow rays caculated */

		RayTracerSettings settings;	/**< The settings used to redner the scene */
	};
}