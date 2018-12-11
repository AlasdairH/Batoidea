#include "Object.h"

namespace Batoidea
{
	Intersect Object::intersect(const Ray &_ray, Intersect &_limits)
	{
		/*
		Intersect intersect;
		for (unsigned int i = 0; i < m_tris.size(); ++i)
		{
			intersect = m_tris[i].intersect(_ray);
			if (intersect.t1 != INFINITY || intersect.t2 != INFINITY)
			{
				return intersect;
			}
		}

		return intersect;
		*/

		//float closestIntersect = INFINITY;

		Intersect closestIntersect;
		float overallClosest = INFINITY;

		// get the closest objcet in the ray intersections
		for (unsigned int i = 0; i < m_tris.size(); ++i)
		{
			glm::vec3 normal;
			Intersect intersect = m_tris[i].intersect(Ray(_ray.origin, _ray.direction), Intersect(0.00f, INFINITY, normal));

			if (intersect.t1 > _limits.t1 && intersect.t1 < _limits.t2 && intersect.t1 < overallClosest)
			{
				closestIntersect.t1 = intersect.t1;
				closestIntersect.normal = intersect.normal;
				overallClosest = closestIntersect.t1;
			}
			if (intersect.t2 > _limits.t1 && intersect.t2 < _limits.t2 && intersect.t2 < overallClosest)
			{
				closestIntersect.t2 = intersect.t2;
				closestIntersect.normal = intersect.normal;
				overallClosest = closestIntersect.t2;
			}
		}

		return closestIntersect;
	}
}

