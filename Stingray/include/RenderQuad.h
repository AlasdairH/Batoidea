#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

namespace Batoidea
{
	struct RenderQuad
	{
		RenderQuad() { }
		RenderQuad(glm::vec2 _tlc, glm::vec2 _brc) { tlc = _tlc; brc = _brc; }

		glm::vec2 tlc;	/**< The coordinates of the top left corner of the quad */
		glm::vec2 brc;	/**< The coordinates of the bottom right corner of the quad */
	};
}