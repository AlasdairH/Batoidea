#include "Ray.h"

namespace Batoidea
{
	Ray::Ray()
	{
		origin = glm::vec3(0);
		direction = glm::vec3(0);
	}

	Ray::Ray(glm::vec3 _origin, glm::vec3 _direction)
	{
		origin = _origin;
		direction = _direction;

		// ray -> AABB pre-optimisations
		inverseDirection = 1.0f / direction;
		sign[0] = (inverseDirection.x < 0);
		sign[1] = (inverseDirection.y < 0);
		sign[2] = (inverseDirection.z < 0);
	}
}