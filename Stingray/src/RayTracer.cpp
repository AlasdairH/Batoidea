#include "RayTracer.h"

namespace Batoidea
{
	RayTracer::RayTracer(const RayTracerSettings _settings)
	{
		m_settings = _settings;
	}

	SDL_Surface RayTracer::render(std::vector<Sphere> &_renderables, std::vector<Light> &_lights, SDL_Surface &_surface)
	{
		SDL_LockSurface(&_surface);

		Camera camera(m_settings.renderResolutionWidth, m_settings.renderResolutionHeight, glm::vec3(0, 0, 0));
		
		LOG_MESSAGE("Beginning Raytrace");

		Timer timer;
		// ray tracing the entire scene
		for (int y = m_settings.renderResolutionHeight - 1; y >= 0; --y)
		{
			for (int x = 0; x < m_settings.renderResolutionWidth; ++x)
			{
				Ray cameraRay = camera.getRay(x, y);
				//LOG_MESSAGE("(" << x << ", " << y << ") - (" << cameraRay.direction.x << ", " << cameraRay.direction.y << ")");
				glm::vec3 pixel = trace(cameraRay, _renderables, _lights);
				
				setPixelColour(_surface, x, y, pixel.r * 255, pixel.g * 255, pixel.b * 255);
			}
		}

		SDL_UnlockSurface(&_surface);
		
		LOG_MESSAGE("Raytrace Finished in " << timer.getDuration() << "s");
		//free(image);

		return _surface;
	}

	glm::vec3 RayTracer::trace(const Ray &_ray, std::vector<Sphere> &_objects, std::vector<Light> &_lights)
	{

		float closestIntersect = INFINITY;
		std::shared_ptr<Sphere> closestRenderable;

		
		for (int i = 0; i < _objects.size(); ++i)
		{
			Intersect intersect = _objects[i].intersect(_ray);
			if (intersect.t1 != INFINITY)
			{
				if (intersect.t1 < closestIntersect)
				{
					closestIntersect = intersect.t1;
					closestRenderable = std::make_shared<Sphere>(_objects[i]);
				}
			}
		}

		// the background colour
		glm::vec3 colour = glm::vec3(0.1f, 0.1f, 0.1f);

		if (closestRenderable != nullptr)
		{
			glm::vec3 position = _ray.point_at(closestIntersect);
			glm::vec3 normal = position - closestRenderable->getCentre();
			normal = (1.0f / glm::length(normal) * normal);
			//LOG_MESSAGE(intersect.t1 << ", " << intersect.t2);
			// if there is an intersect
			colour = closestRenderable->getColour();
			colour *= computeLighting(_lights, normal, position);

			// clamp to [0.0:1.0]
			glm::clamp(colour, 0.01f, 0.99f);
		}

		return colour;
	}

	float RayTracer::computeLighting(std::vector<Light> &_lights, glm::vec3 _normal, glm::vec3 _position)
	{
		// the running intensity, will be added to by each light that affects this point
		float intensity = 0;
		float lengthNormal = glm::length(_normal);

		for (unsigned int i = 0; i < _lights.size(); ++i)
		{
			glm::vec3 distance = _lights[i].position - _position;

			float n_dot_l = glm::dot(_normal, distance);
			if (n_dot_l > 0)
			{
				intensity += _lights[i].intensity * n_dot_l / (lengthNormal * glm::length(distance));
			}
		}
		return intensity;
	}

	void RayTracer::setPixelColour(SDL_Surface &_surface, const int _x, const int _y, const int _r, const int _g, const int _b)
	{
		SDL_LockSurface(&_surface);

		Uint32 *pixels = (Uint32*)_surface.pixels;
		pixels[_x + m_settings.renderResolutionWidth * _y] = (_r << 16) | (_g << 8) | _b;

		SDL_UnlockSurface(&_surface);
	}
}