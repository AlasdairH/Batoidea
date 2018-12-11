#include "Triangle.h"

namespace Batoidea
{
	Triangle::Triangle()
	{
		verts[0] = glm::vec3(0);
		verts[1] = glm::vec3(0);
		verts[2] = glm::vec3(0);
	}

	Triangle::Triangle(const glm::vec3 &_v0, const glm::vec3 &_v1, const glm::vec3 &_v2)
	{
		verts[0] = _v0;
		verts[1] = _v1;
		verts[2] = _v2;
	}

	Intersect Triangle::intersect(const Ray & _ray, const Intersect &_limits)
	{
		Intersect intersect(INFINITY, INFINITY, glm::vec3(0));

		// compute plane's normal
		glm::vec3 v0v1 = verts[1] - verts[0];
		glm::vec3 v0v2 = verts[2] - verts[0];
		// no need to normalize
		glm::vec3 N = glm::cross(v0v1, v0v2); // N 
		float area2 = N.length();

		// Step 1: finding P

		// check if ray and plane are parallel ?
		float NdotRayDirection = glm::dot(N, _ray.direction);
		if (fabs(NdotRayDirection) < kEpsilon) // almost 0 
			return intersect; // they are parallel so they don't intersect ! 

		// compute d parameter using equation 2
		float d = glm::dot(N, verts[0]);

		// compute t (equation 3)
		float t = (glm::dot(N, _ray.origin) + d) / NdotRayDirection;
		// check if the triangle is in behind the ray
		if (t < 0) return intersect; // the triangle is behind 

		// compute the intersection point using equation 1
		glm::vec3 P = _ray.point_at(t);

		// Step 2: inside-outside test
		glm::vec3 C; // vector perpendicular to triangle's plane 

		// edge 0
		glm::vec3 edge0 = verts[1] - verts[0];
		glm::vec3 vp0 = P - verts[0];
		C = glm::cross(edge0, vp0);
		if (glm::dot(N, C) < 0) return intersect; // P is on the right side 

		// edge 1
		glm::vec3 edge1 = verts[2] - verts[1];
		glm::vec3 vp1 = P - verts[1];
		C = glm::cross(edge1, vp1);
		if (glm::dot(N, C) < 0)  return intersect; // P is on the right side 

		// edge 2
		glm::vec3 edge2 = verts[0] - verts[2];
		glm::vec3 vp2 = P - verts[2];
		C = glm::cross(edge2, vp2);
		if (glm::dot(N, C) < 0) return intersect; // P is on the right side; 

		return Intersect(t, INFINITY, N); // this ray hits the triangle 
	}
}