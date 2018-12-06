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
	class Timer
	{
	public:
		Timer() { m_start = std::clock(); }

		void reset() { m_start = std::clock(); }
		void saveDuration() { m_savedTime = getDuration(); }
		float getDuration() { return (std::clock() - m_start) / (float)CLOCKS_PER_SEC; }
		float getSavedDuration() { return m_savedTime; }

	protected:
		std::clock_t	m_start;
		float			m_savedTime = -1.0f;
	};
}