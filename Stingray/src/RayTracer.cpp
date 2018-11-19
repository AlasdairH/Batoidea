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
		
		glm::vec3 lowerLeft = glm::vec3(-2.0f, -1.0f, -1.0f);
		glm::vec3 horizontal = glm::vec3(4.0f, 0.0f, 0.0f);
		glm::vec3 vertical = glm::vec3(0.0f, 2.0f, 0.0f);
		glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
		LOG_MESSAGE("Beginning Raytrace");

		Timer timer;
		// ray tracing the entire scene
		for (int y = m_settings.resolution_height - 1; y >= 0; --y)
		{
			for (int x = 0; x < m_settings.resolution_width; ++x)
			{
				LOG_MESSAGE(y << ", " << x);
				// for pixel x, y
				float u = x / float(m_settings.resolution_width);
				float v = y / float(m_settings.resolution_height);
				Ray ray(origin, lowerLeft + u * horizontal + v * vertical);
				
				glm::vec3 pixel = trace(ray, _renderables);
				
				setPixelColour(_surface, x, y, pixel.r * 255, pixel.g * 255, pixel.b * 255);
			}
		}

		SDL_UnlockSurface(&_surface);
		
		LOG_MESSAGE("Raytrace Finished in " << timer.getDuration() << "s");
		free(image);

		return _surface;
	}

	glm::vec3 RayTracer::trace(const Ray &_ray, std::vector<Sphere> &_objects)
	{
		/*
		for (unsigned int i = 0; i < _objects.size(); ++i)
		{
			float intersect = _objects[i].intersect(_ray);
			if (intersect > 0.0f)
			{
				//LOG_MESSAGE(intersect);
				return glm::vec3(1.0f, 0.0f, 0.0f);
			}
		}
		*/

		glm::vec3 unitDirection = glm::normalize(_ray.direction);
		float t = 0.5 * unitDirection.y + 1.0f;
		glm::vec3 returnValue = (1.0f - t) * glm::vec3(1.0f, 1.0f, 1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
		LOG_MESSAGE("(" << unitDirection.x << ", " << unitDirection.y << ", " << unitDirection.z << ") |" << t << " | " << "Colour: (" << returnValue.x * 255 << ", " << returnValue.y * 255 << ", " << returnValue.z * 255 << ")");
		return returnValue;
	}

	void RayTracer::setPixelColour(SDL_Surface &_surface, const int _x, const int _y, const int _r, const int _g, const int _b)
	{
		SDL_LockSurface(&_surface);

		Uint32 *pixels = (Uint32*)_surface.pixels;
		pixels[_x + m_settings.resolution_width * _y] = (_r << 16) | (_g << 8) | _b;

		SDL_UnlockSurface(&_surface);
	}
}