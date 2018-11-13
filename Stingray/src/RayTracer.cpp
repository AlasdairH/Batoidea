#include "RayTracer.h"

namespace Batoidea
{
	RayTracer::RayTracer(const RayTracerSettings _settings)
	{
		m_settings = _settings;
	}

	SDL_Surface RayTracer::render(std::vector<Sphere> &_renderables, SDL_Surface &_surface)
	{
		SDL_LockSurface(&_surface);
		//SDL_Surface surface;

		glm::vec3 *image = new glm::vec3[m_settings.resolution_width * m_settings.resolution_height];
		float invWidth = 1 / float(m_settings.resolution_width), invHeight = 1 / float(m_settings.resolution_height);
		float angle = tan(M_PI * 0.5 * m_settings.fov / 180.0f);
		float aspectRatio = m_settings.resolution_width / float(m_settings.resolution_height);

		LOG_MESSAGE("Beginning Raytrace");

		for (int y = 0; y < m_settings.resolution_height; ++y)
		{
			for (int x = 0; x < m_settings.resolution_width; ++x)
			{
				float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectRatio;
				float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
				glm::vec3 raydir(xx, yy, -1);
				raydir = glm::normalize(raydir);
				//*pixel = trace(Ray(glm::vec3(0), raydir), _renderables, 0);
			}
		}

		SDL_memset(_surface.pixels, 0, (_surface.h / 2) * _surface.pitch);

		SDL_UnlockSurface(&_surface);
		return _surface;
	}

	glm::vec3 RayTracer::trace(const Ray _ray, const std::vector<Sphere>& spheres, const int & depth)
	{
		//if (_ray.direction.length() != 1) std::cerr << "Error " << _ray.direction << std::endl;
		float tnear = INFINITY;
		const Sphere* sphere = NULL;
		// find intersection of this ray with the sphere in the scene
		for (unsigned i = 0; i < spheres.size(); ++i) {
			float t0 = INFINITY, t1 = INFINITY;
			if (spheres[i].intersect(_ray.origin, _ray.direction, t0, t1)) {
				if (t0 < 0) t0 = t1;
				if (t0 < tnear) {
					tnear = t0;
					sphere = &spheres[i];
				}
			}
		}
		// if there's no intersection return black or background color
		if (!sphere) return glm::vec3(2);
		glm::vec3 surfaceColor; // color of the ray/surfaceof the object intersected by the ray 
		glm::vec3 phit = _ray.origin + _ray.direction * tnear; // point of intersection 
		glm::vec3 nhit = phit - sphere->centre; // normal at the intersection point 
		nhit = glm::normalize(nhit); // normalize normal direction 
		// If the normal and the view direction are not opposite to each other
		// reverse the normal direction. That also means we are inside the sphere so set
		// the inside bool to true. Finally reverse the sign of IdotN which we want
		// positive.
		float bias = 1e-4; // add some bias to the point from which we will be tracing 
		bool inside = false;
		if (glm::dot(_ray.direction, nhit) > 0) nhit = -nhit, inside = true;
		if ((sphere->transparency > 0 || sphere->reflection > 0) && depth < m_settings.maxRayDepth) {
			float facingratio = -glm::dot(_ray.direction, nhit);
			// change the mix value to tweak the effect
			float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);
			// compute reflection direction (not need to normalize because all vectors
			// are already normalized)
			glm::vec3 refldir = _ray.direction - nhit * 2.0f * glm::dot(_ray.direction, nhit);
			refldir = glm::normalize(refldir);
			glm::vec3 reflection = trace(Ray(phit + nhit * bias, refldir), spheres, depth + 1);
			glm::vec3 refraction;
			// if the sphere is also transparent compute refraction ray (transmission)
			if (sphere->transparency) {
				float ior = 1.1, eta = (inside) ? ior : 1 / ior; // are we inside or outside the surface? 
				float cosi = -glm::dot(nhit, _ray.direction);
				float k = 1 - eta * eta * (1 - cosi * cosi);
				glm::vec3 refrdir = _ray.direction * eta + nhit * (eta *  cosi - sqrt(k));
				refrdir = glm::normalize(refrdir);
				refraction = trace(Ray(phit - nhit * bias, refrdir), spheres, depth + 1);
			}
			// the result is a mix of reflection and refraction (if the sphere is transparent)
			surfaceColor = (
				reflection * fresneleffect +
				refraction * (1 - fresneleffect) * sphere->transparency) * sphere->surfaceColour;
		}
		else {
			// it's a diffuse object, no need to raytrace any further
			for (unsigned i = 0; i < spheres.size(); ++i) {
				if (spheres[i].emissionColour.x > 0) {
					// this is a light
					glm::vec3 transmission = glm::vec3(1, 1, 1);
					glm::vec3 lightDirection = spheres[i].centre - phit;
					lightDirection = glm::normalize(lightDirection);
					for (unsigned j = 0; j < spheres.size(); ++j) {
						if (i != j) {
							float t0, t1;
							if (spheres[j].intersect(phit + nhit * bias, lightDirection, t0, t1)) {
								transmission = glm::vec3(0, 0, 0);
								break;
							}
						}
					}
					surfaceColor += sphere->surfaceColour * transmission *
						std::max(float(0), glm::dot(nhit, lightDirection)) * spheres[i].emissionColour;
				}
			}
		}

		return surfaceColor + sphere->emissionColour;
	}
}