#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

namespace Batoidea
{
	class Material
	{
	public:
		Material() { };

		glm::vec3 colourDiffuse = glm::vec3(1.0f, 1.0f, 1.0f);

		float shine = 1.0f;


		float reflectiveness = 0.0f;

	};
}