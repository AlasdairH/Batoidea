#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

namespace Batoidea
{
	/*! @class Material
	*	@brief A material that can be used when rendering objects
	*
	*	A material is used when rendering an object to give it properties such as colour, shine or reflectiveness.
	*/
	struct Material
	{
	public:
		glm::vec3 colourDiffuse	 = glm::vec3(1.0f, 1.0f, 1.0f);		/**< The diffuse colour */

		float shine				= 1.0f;								/**< The shine for the specular calculations */
		float reflectiveness	= 0.0f;								/**< The reflectivity (Range: [0.0:1.0]) */

	};
}