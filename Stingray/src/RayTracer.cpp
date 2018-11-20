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

		glm::vec3 *image = new glm::vec3[m_settings.renderResolutionWidth * m_settings.renderResolutionHeight];
		
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
				//LOG_MESSAGE(y << ", " << x);
				// for pixel x, y
				float u = x / float(m_settings.renderResolutionWidth);
				float v = y / float(m_settings.renderResolutionHeight);
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
		glm::vec3 colour = glm::vec3(1.0f, 0.0f, 0.0f);
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