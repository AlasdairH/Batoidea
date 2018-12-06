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
		LOG_MESSAGE("Calculating Render Quads");
		std::vector<RenderQuad> renderQuads = calculateRenderQuads();

		// transfer objects and lights to raytracer for ease of multi-threaded access
		m_objects = _renderables;
		m_lights = _lights;
		m_pixels = (Uint32*)_surface.pixels;
		
		LOG_MESSAGE("Beginning Raytrace");

		m_timer.reset();
		SDL_LockSurface(&_surface);
		// ray tracing the entire scene
		for (unsigned int i = 0; i < renderQuads.size(); ++i)
		{
			m_threadPool->enqueue([=]
			{
				renderQuadToPixels(renderQuads[i].tlc, renderQuads[i].brc);
			});
		}

		SDL_UnlockSurface(&_surface);

		//free(image);

		return _surface;
	}

	bool RayTracer::isRenderComplete()
	{
		if (m_threadPool->isTasksEmpty())
		{
			m_timer.saveDuration();
			return true;
		}

		return false;
	}

	std::vector<RenderQuad> RayTracer::calculateRenderQuads()
	{
		std::vector<RenderQuad> renderQuads;
		// get the width and height of each quad
		int quadWidth = std::floor(m_settings.renderResolutionWidth / (float)m_settings.renderQuadResolutionWidth);
		int quadHeight = std::floor(m_settings.renderResolutionHeight / (float)m_settings.renderQuadResolutionHeight);
		// add render quads for the width of the image
		for (int x = 0; x < m_settings.renderQuadResolutionWidth; ++x)
		{
			for (int y = 0; y < m_settings.renderQuadResolutionHeight; ++y)
			{
				renderQuads.push_back(RenderQuad(glm::vec2(x * quadWidth, y * quadHeight), glm::vec2((x * quadWidth) + quadWidth, (y * quadHeight) + quadHeight)));
			}
		}

		// filling in any border due to there being a remainder on the quad width. We'll just add another few quads to fill the space
		int widthRemainder = m_settings.renderQuadResolutionWidth - (m_settings.renderQuadResolutionWidth * quadWidth);
		int heightRemainder = m_settings.renderQuadResolutionHeight - (m_settings.renderQuadResolutionHeight * quadHeight);
		// correct for width
		if (widthRemainder != 0)
		{
			for (int y = 0; y < m_settings.renderQuadResolutionHeight; ++y)
			{
				renderQuads.push_back(RenderQuad(glm::vec2(m_settings.renderQuadResolutionWidth * quadWidth, y * quadHeight), glm::vec2(m_settings.renderResolutionWidth, (y * quadHeight) + quadHeight)));
			}
		}
		// correct for height
		if (heightRemainder != 0)
		{
			for (int x = 0; x < m_settings.renderQuadResolutionWidth; ++x)
			{
				renderQuads.push_back(RenderQuad(glm::vec2(x * quadWidth, m_settings.renderQuadResolutionHeight * quadHeight), glm::vec2((x * quadWidth) + quadWidth, m_settings.renderResolutionHeight)));
			}
			// doesn't really matter which remainder check this goes in as long as it is
			// fill the little un-rendered block at the bottom right of the screen
			renderQuads.push_back(RenderQuad(glm::vec2(m_settings.renderQuadResolutionWidth * quadWidth, m_settings.renderQuadResolutionHeight * quadHeight), glm::vec2(m_settings.renderResolutionWidth, m_settings.renderResolutionHeight)));
		}

		return renderQuads;
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
					// lock  m_pixels when writing, this is not strictly required as all threads should be writing to different parts of the pixel pointer
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