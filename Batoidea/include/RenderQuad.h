#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

namespace Batoidea
{
	/*! @class RenderQuad
	*	@brief A simple quad used to define a segment of the render image
	*
	*	The RenderQuad is used to give a thread to a specific segment of the screen to render.
	*/
	struct RenderQuad
	{
		/** @brief RenderQuad Constructor
		*
		*	Creates a default RenderQuad
		*/
		RenderQuad() { }
		/** @brief RenderQuad Constructor
		*	@param _tlc The top left corner of the quad
		*	@param _brc The bototm right corner of the quad
		*
		*	Creates a RenderQuad with the specified arguments
		*/
		RenderQuad(glm::vec2 _tlc, glm::vec2 _brc) { tlc = _tlc; brc = _brc; }

		glm::vec2 tlc;	/**< The coordinates of the top left corner of the quad */
		glm::vec2 brc;	/**< The coordinates of the bottom right corner of the quad */
	};
}