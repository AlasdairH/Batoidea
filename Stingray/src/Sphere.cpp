#include "Sphere.h"

namespace Batoidea
{

	float Sphere::intersect(const Ray &_ray)
	{
		/*
		glm::vec3 oc = _ray.origin - m_centre;
		float a = glm::dot(_ray.direction, _ray.direction);
		float b = 2.0 * glm::dot(oc, _ray.direction);
		float c = glm::dot(oc, oc) - m_radius * m_radius;
		float discriminant = b * b - 4 * a * c;
		if (discriminant < 0)
		{
			return -1.0f;
		}
		else
		{
			return (-b - sqrt(discriminant)) / (2.0f * a);
		}
		*/

		// geometric solution
		float t, t0, t1;
		float radius2 = pow(m_radius, 2);

		glm::vec3 L = m_centre - _ray.origin;
		float tca = glm::dot(L, _ray.direction);
		// if (tca < 0) return false;
		float d2 = glm::dot(L, L) - tca * tca;
		if (d2 > radius2) return false;
		float thc = sqrt(radius2 - d2);
		t0 = tca - thc;
		t1 = tca + thc;

		if (t0 > t1) std::swap(t0, t1);

		if (t0 < 0) 
		{
			t0 = t1; // if t0 is negative, let's use t1 instead 
			if (t0 < 0) return false; // both t0 and t1 are negative 
		}

		t = t0;
		LOG_MESSAGE(t);
		return t;
	}
}
