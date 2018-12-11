#include "BoundingBox.h"

namespace Batoidea
{
	bool BoundingBox::intersect(const Ray &_ray)
	{
		float tmin, tmax, tymin, tymax, tzmin, tzmax;

		tmin = (bounds[_ray.sign[0]].x - _ray.origin.x) * _ray.inverseDirection.x;
		tmax = (bounds[1 - _ray.sign[0]].x - _ray.origin.x) * _ray.inverseDirection.x;
		tymin = (bounds[_ray.sign[1]].y - _ray.origin.y) * _ray.inverseDirection.y;
		tymax = (bounds[1 - _ray.sign[1]].y - _ray.origin.y) * _ray.inverseDirection.y;

		if ((tmin > tymax) || (tymin > tmax))
			return false;

		if (tymin > tmin)
			tmin = tymin;
		if (tymax < tmax)
			tmax = tymax;

		tzmin = (bounds[_ray.sign[2]].z - _ray.origin.z) * _ray.inverseDirection.z;
		tzmax = (bounds[1 - _ray.sign[2]].z - _ray.origin.z) * _ray.inverseDirection.z;

		if ((tmin > tzmax) || (tzmin > tmax))
			return false;

		if (tzmin > tmin)
			tmin = tzmin;
		if (tzmax < tmax)
			tmax = tzmax;

		float t = tmin;

		if (t < 0) {
			t = tmax;
			if (t < 0) return false;
		}

		return true;
	}
}