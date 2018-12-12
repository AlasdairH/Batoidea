#pragma once

// cstdlib
#include <iostream>
#include <cstdio>
#include <ctime>

// external libs

// program
#include "Logger.h"

// TODO: Doxygen

namespace Batoidea
{
	/*! @class Timer
	*	@brief A timer class for measuring time in seconds from a start point
	*
	*	Used for timing operations. Will start the timer on construction and can be polled for the duration at any time.
	*	This timer isn't particularly accurate but it's good enough for basic profiling.
	*/
	class Timer
	{
	public:
		/** @brief Timer Constructor
		*
		*	Creates a timer and sets the start time
		*/
		Timer() { m_start = std::clock(); }

		/** @brief Resets the timer to 0
		*
		*	Resets the timer back to 0, any duraction gathered after calling this will be from this new time.
		*/
		void reset() { m_start = std::clock(); }
		/** @brief Saves the current duration to the saved time member variable
		*
		*	Saves the current duration to the saved time member variable.
		*/
		void saveDuration() { m_savedTime = getDuration(); }
		/** @brief Gets the current duration
		*	@return The time passed in seconds since the start of the clock
		*
		*	Gets the duration the clock has been running for.
		*/
		float getDuration() { return (std::clock() - m_start) / (float)CLOCKS_PER_SEC; }
		/** @brief Gets the saved duration
		*	@return The saved duration
		*
		*	Returns the saved duration saved by saveDuration. If no time has been saved, returns -1.
		*/
		float getSavedDuration() { return m_savedTime; }

	protected:
		std::clock_t	m_start;				/**< The time when the clock as started */
		float			m_savedTime = -1.0f;	/**< The saved duration */
	};
}