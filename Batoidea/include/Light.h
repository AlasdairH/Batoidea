#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "Logger.h"

namespace Batoidea
{
	enum LightType { LIGHT_POINT, LIGHT_DIRECTIONAL };

	/*! @class Light
	*	@brief A light in 3D space
	*
	*	This class holds the data for a light in 3D space, either point or directional.
	*/
	struct Light
	{
	public:
		/** @brief Light Constructor
		*
		*	Creates a default light
		*/
		Light() { }

		/** @brief Light Constructor
		*	@param _pos The position of the light. Also doubles as the direction for directional lights by default.
		*	@param _int The intensity of the light in the range [0.0:1.0]
		*
		*	Creates a light with the specified arguments
		*/
		Light(glm::vec3 _pos, float _int)
		{
			position = _pos;
			direction = _pos;
			intensity = _int;
		}

		LightType type = LIGHT_POINT;

		glm::vec3 position;									/**< The position of the light (default 0, 0, 0) */
		glm::vec3 direction		= glm::vec3(0, -1, 0);		/**< The direction of the light (if directional) */
		float intensity			= 1.0f;						/**< The intensity of the light */
	};
}