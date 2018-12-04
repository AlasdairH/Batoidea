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

		Camera camera(m_settings.renderResolutionWidth, m_settings.renderResolutionHeight, glm::vec3(0, 1, -5));
		Light light(glm::vec3(0, 0, 0), glm::vec4(1, 1, 1, 0));

		//glm::vec3 *image = new glm::vec3[m_settings.renderResolutionWidth * m_settings.renderResolutionHeight];
		
		glm::vec3 lowerLeft = glm::vec3(-2.0f, -1.0f, -1.0f);
		glm::vec3 horizontal = glm::vec3(4.0f, 0.0f, 0.0f);
		glm::vec3 vertical = glm::vec3(0.0f, 2.0f, 0.0f);
		glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
		LOG_MESSAGE("Beginning Raytrace");

		Timer timer;
		// ray tracing the entire scene
		for (int y = m_settings.renderResolutionHeight - 1; y >= 0; --y)
		{
			for (int x = 0; x < m_settings.renderResolutionWidth; ++x)
			{
				Ray cameraRay = camera.getRay(x, y);
				//LOG_MESSAGE("(" << x << ", " << y << ") - (" << cameraRay.direction.x << ", " << cameraRay.direction.y << ")");
				glm::vec3 pixel = trace(cameraRay, _renderables);
				
				setPixelColour(_surface, x, y, pixel.r * 255, pixel.g * 255, pixel.b * 255);
			}
		}

		SDL_UnlockSurface(&_surface);
		
		LOG_MESSAGE("Raytrace Finished in " << timer.getDuration() << "s");
		//free(image);

		return _surface;
	}

	glm::vec3 RayTracer::trace(const Ray &_ray, std::vector<Sphere> &_objects)
	{
		glm::vec3 colour;
		for (int i = 0; i < _objects.size(); ++i)
		{
			Intersect intersect = _objects[i].intersect(_ray);
			if (intersect.t1 != INFINITY || intersect.t2 != INFINITY)
			{
				//LOG_MESSAGE(intersect.t1 << ", " << intersect.t2);
				// if there is an intersect
				colour = _objects[i].getColour();
			}
		}
		return colour;
	}

	void RayTracer::setPixelColour(SDL_Surface &_surface, const int _x, const int _y, const int _r, const int _g, const int _b)
	{
		SDL_LockSurface(&_surface);

		Uint32 *pixels = (Uint32*)_surface.pixels;
		pixels[_x + m_settings.renderResolutionWidth * _y] = (_r << 16) | (_g << 8) | _b;

		SDL_UnlockSurface(&_surface);
	}
}