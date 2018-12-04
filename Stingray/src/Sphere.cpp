#include "Sphere.h"

namespace Batoidea
{

	Intersect Sphere::intersect(const Ray &_ray)
	{
		glm::vec3 OC = _ray.origin - m_centre;

		float k1 = dot(_ray.direction, _ray.direction);
		float k2 = 2 * glm::dot(OC, _ray.direction);
		float k3 = glm::dot(OC, OC) - m_radius * m_radius;

		float discriminant = k2 * k2 - 4 * k1*k3;
		if (discriminant < 0)
			return Intersect();

		float t1 = (-k2 + sqrt(discriminant)) / (2 * k1);
		float t2 = (-k2 - sqrt(discriminant)) / (2 * k1);
		return Intersect(t1, t2);
	}
}
