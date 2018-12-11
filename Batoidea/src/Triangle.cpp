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
		float t, u, v;

#define MOLLER_TRUMBORE
#define CULLING
#ifdef MOLLER_TRUMBORE 
		glm::vec3 v0v1 = verts[1] - verts[0];
		glm::vec3 v0v2 = verts[2] - verts[0];
		glm::vec3 pvec = glm::cross(_ray.direction, v0v2);
		float det = glm::dot(v0v1, pvec);

		glm::vec3 N = glm::cross(v0v1, v0v2); // N 
#ifdef CULLING 
		// if the determinant is negative the triangle is backfacing
		// if the determinant is close to 0, the ray misses the triangle
		if (det < kEpsilon) return intersect;
#else 
		// ray and triangle are parallel if det is close to 0
		if (fabs(det) < kEpsilon) return false;
#endif 
		float invDet = 1 / det;

		glm::vec3 tvec = _ray.origin - verts[0];
		u = glm::dot(tvec, pvec) * invDet;
		if (u < 0 || u > 1) return intersect;

		glm::vec3 qvec = glm::cross(tvec, v0v1);
		v = glm::dot(_ray.direction, qvec) * invDet;
		if (v < 0 || u + v > 1) return intersect;

		t = glm::dot(v0v2, qvec) * invDet;

		return Intersect(t, t, N); // this ray hits the triangle 

#endif
	}
}