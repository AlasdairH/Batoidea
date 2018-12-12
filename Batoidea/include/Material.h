#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"
#include "Texture.h"

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
		Material() { diffuse = std::make_shared<Texture>(512, 512); }

		std::shared_ptr<Texture>	diffuse;						/**< The diffuse colour */

		float						shine				= 1.0f;		/**< The shine for the specular calculations */
		float						reflectiveness		= 0.0f;		/**< The reflectivity (Range: [0.0:1.0]) */

	};
}