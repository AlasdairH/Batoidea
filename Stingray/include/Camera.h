#pragma once

// cstdlib

// external libs
#include "GLM/common.hpp"
#include "GLM/geometric.hpp"

// program
#include "Logger.h"
#include "Ray.h"

namespace Batoidea
{
	class Camera
	{
	public:
		Camera(const int _viewportResolutionWidth, const int _viewportResolutionHeight);

		Ray getRay(const int _x, const int _y);

	protected:
		int m_viewportResolutionWidth;
		int m_viewportResolutionHeight;
	};
}
