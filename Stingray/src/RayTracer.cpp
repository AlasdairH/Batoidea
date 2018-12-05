#include "RayTracer.h"

namespace Batoidea
{
	RayTracer::RayTracer(const RayTracerSettings _settings)
	{
		m_settings = _settings;

		m_threadPool = std::make_shared<Threads::ThreadPool>(std::thread::hardware_concurrency());
		
		m_camera = std::make_shared<Camera>(m_settings.renderResolutionWidth, m_settings.renderResolutionHeight, glm::vec3(0, 0, 0));
	}

	SDL_Surface RayTracer::render(std::vector<Sphere> &_renderables, std::vector<Light> &_lights, SDL_Surface &_surface)
	{
		std::vector<RenderQuad> renderQuads;
		
		// transfer objects and lights to raytracer for ease of multi-threaded access
		m_objects = _renderables;
		m_lights = _lights;
		m_pixels = (Uint32*)_surface.pixels;
		
		LOG_MESSAGE("Beginning Raytrace");
		Timer timer;
		SDL_LockSurface(&_surface);
		// ray tracing the entire scene
		for (unsigned int i = 0; i < renderQuads.size(); ++i)
		{
			renderQuadToPixels(renderQuads[i].tlc, renderQuads[i].brc);
		}

		SDL_UnlockSurface(&_surface);
		
		
		LOG_MESSAGE("Raytrace Finished in " << timer.getDuration() << "s");
		//free(image);

		return _surface;
	}

	void RayTracer::renderQuadToPixels(const glm::vec2 _start, const glm::vec2 _finish)
	{
		for (int y = _start.y; y < _finish.y; ++y)
		{
			for (int x = _start.x; x < _finish.x; ++x)
			{
				Ray cameraRay = m_camera->getRay(x, y);
				glm::vec3 pixelColour = trace(cameraRay);
				pixelColour *= 255;

				//LOG_MESSAGE((int)pixelColour.r << ", " << (int)pixelColour.g << ", " << (int)pixelColour.b);
				
				{
					// lock  m_pixels when writing
					std::lock_guard<std::mutex> pixelLock(m_pixelMutex);
					m_pixels[x + m_settings.renderResolutionWidth * y] = ((int)pixelColour.r << 16) | ((int)pixelColour.g << 8) | (int)pixelColour.b;
				}

			}
		}
	}

	glm::vec3 RayTracer::trace(const Ray &_ray)
	{

		float closestIntersect = INFINITY;
		std::shared_ptr<Sphere> closestRenderable;

		for (unsigned int i = 0; i < m_objects.size(); ++i)
		{
			Intersect intersect = m_objects[i].intersect(_ray);
			if (intersect.t1 != INFINITY)
			{
				if (intersect.t1 < closestIntersect)
				{
					closestIntersect = intersect.t1;
					closestRenderable = std::make_shared<Sphere>(m_objects[i]);
				}
			}
		}

		// the background colour
		glm::vec3 colour = m_settings.backgroundColour;

		if (closestRenderable != nullptr)
		{
			glm::vec3 position = _ray.point_at(closestIntersect);
			glm::vec3 normal = position - closestRenderable->getCentre();
			normal = (1.0f / glm::length(normal) * normal);
			//LOG_MESSAGE(intersect.t1 << ", " << intersect.t2);
			// if there is an intersect
			colour = closestRenderable->getColour();
			colour *= computeLighting(normal, position);

			// clamp to [0.0:1.0]
			glm::clamp(colour, 0.01f, 0.99f);
		}

		return colour;
	}

	float RayTracer::computeLighting(glm::vec3 _normal, glm::vec3 _position)
	{
		// the running intensity, will be added to by each light that affects this point
		float intensity = 0;
		float lengthNormal = glm::length(_normal);
		glm::vec3 L;

		for (unsigned int i = 0; i < m_lights.size(); ++i)
		{
			if (m_lights[i].type == LIGHT_POINT)
			{
				L = m_lights[i].position - _position;
			}
			else if (m_lights[i].type == LIGHT_DIRECTIONAL)
			{
				L = m_lights[i].direction;
			}

			float n_dot_l = glm::dot(_normal, L);
			if (n_dot_l > 0)
			{
				intensity += m_lights[i].intensity * n_dot_l / (lengthNormal * glm::length(L));
			}
		}
		return intensity;
	}
}