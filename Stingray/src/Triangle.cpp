#include "Triangle.h"

namespace Batoidea
{
	Triangle::Triangle(const glm::vec3 &_v0, const glm::vec3 &_v1, const glm::vec3 &_v2)
	{
		m_v0 = _v0;
		m_v1 = _v1;
		m_v2 = _v2;
	}

	Intersect Triangle::intersect(const Ray & _ray)
	{
		Intersect intersect(INFINITY, INFINITY);

		// compute plane's normal
		glm::vec3 v0v1 = m_v1 - m_v0;
		glm::vec3 v0v2 = m_v2 - m_v0;
		// no need to normalize
		glm::vec3 N = glm::cross(v0v1, v0v2); // N 
		float area2 = N.length();

		// Step 1: finding P

		// check if ray and plane are parallel ?
		float NdotRayDirection = glm::dot(N, _ray.direction);
		if (fabs(NdotRayDirection) < kEpsilon) // almost 0 
			return intersect; // they are parallel so they don't intersect ! 

		// compute d parameter using equation 2
		float d = glm::dot(N, m_v0);

		// compute t (equation 3)
		float t = (glm::dot(N, _ray.origin) + d) / NdotRayDirection;
		// check if the triangle is in behind the ray
		if (t < 0) return intersect; // the triangle is behind 

		// compute the intersection point using equation 1
		glm::vec3 P = _ray.point_at(t);

		// Step 2: inside-outside test
		glm::vec3 C; // vector perpendicular to triangle's plane 

		// edge 0
		glm::vec3 edge0 = m_v1 - m_v0;
		glm::vec3 vp0 = P - m_v0;
		C = glm::cross(edge0, vp0);
		if (glm::dot(N, C) < 0) return intersect; // P is on the right side 

		// edge 1
		glm::vec3 edge1 = m_v2 - m_v1;
		glm::vec3 vp1 = P - m_v1;
		C = glm::cross(edge1, vp1);
		if (glm::dot(N, C) < 0)  return intersect; // P is on the right side 

		// edge 2
		glm::vec3 edge2 = m_v0 - m_v2;
		glm::vec3 vp2 = P - m_v2;
		C = glm::cross(edge2, vp2);
		if (glm::dot(N, C) < 0) return intersect; // P is on the right side; 

		return Intersect(t, INFINITY); // this ray hits the triangle 
	}
}