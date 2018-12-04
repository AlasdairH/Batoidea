#pragma once

// cstdlib

// external libs
#include "GLM/common.hpp"

// program
#include "Logger.h"

namespace Batoidea
{
	class Light
	{
	public:
		Light();
		Light(const glm::vec3 &_position, const glm::vec4 &_colour);

		inline glm::vec3 getPosition() { return m_position; }
		inline glm::vec3 getColour() { return m_colour; }

	protected:
		glm::vec3 m_position;
		glm::vec4 m_colour;
	};
}