#include "RayTracer.h"

namespace Batoidea
{
	RayTracer::RayTracer(const RayTracerSettings _settings)
	{
		m_settings = _settings;

		m_threadPool = std::make_shared<Threads::ThreadPool>(m_settings.threads);
		
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

	void RayTracer::calculateClosestIntersection(std::shared_ptr<Sphere>& _sphere, float & _closestIntersection, const Ray & _ray, Intersect _limits)
	{
		float closestIntersect = INFINITY;
		std::shared_ptr<Sphere> closestRenderable = nullptr;

		// get the closest objcet in the ray intersections
		for (unsigned int i = 0; i < m_objects.size(); ++i)
		{
			Intersect intersect = m_objects[i].intersect(Ray(_ray.origin, _ray.direction));

			
			if (intersect.t1 > _limits.t1 && intersect.t1 < _limits.t2 && intersect.t1 < closestIntersect)
			{
				closestIntersect = intersect.t1;
				//LOG_MESSAGE(closestIntersect);
				closestRenderable = std::make_shared<Sphere>(m_objects[i]);
			}
			if (intersect.t2 > _limits.t1 && intersect.t2 < _limits.t2 && intersect.t2 < closestIntersect)
			{
				closestIntersect = intersect.t2;
				//LOG_MESSAGE(closestIntersect);
				closestRenderable = std::make_shared<Sphere>(m_objects[i]);
			}
			

			/*
			if (intersect.t1 != INFINITY && intersect.t1 > 0.0f)
			{
				if (intersect.t1 < closestIntersect && _limits.t1 < intersect.t1 < _limits.t2)
				{
					closestIntersect = intersect.t1;
					closestRenderable = std::make_shared<Sphere>(m_objects[i]);
				}
				if (intersect.t2 < closestIntersect && _limits.t1 < intersect.t2 < _limits.t2 )
				{
					closestIntersect = intersect.t2;
					closestRenderable = std::make_shared<Sphere>(m_objects[i]);
				}
			}
			*/
		}

		_sphere = closestRenderable;
		_closestIntersection = closestIntersect;
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
		for (int y = (int)_start.y; y < (int)_finish.y; ++y)
		{
			for (int x = (int)_start.x; x < (int)_finish.x; ++x)
			{
				Ray cameraRay = m_camera->getRay(x, y);
				glm::vec3 pixelColour = trace(cameraRay, m_settings.reflectionRecursionDepth, Intersect(0.0f, 100.0f));
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

	glm::vec3 RayTracer::trace(const Ray &_ray, int _depth, Intersect _limits)
	{
		float closestIntersect = INFINITY;
		std::shared_ptr<Sphere> closestRenderable;
		calculateClosestIntersection(closestRenderable, closestIntersect, _ray, _limits);

		// the background colour
		glm::vec3 localColour = m_settings.backgroundColour;

		if (closestRenderable == nullptr)
		{
			return localColour;
		}

		glm::vec3 position = _ray.point_at(closestIntersect);
		glm::vec3 normal = position - closestRenderable->getCentre();
		normal = normal / glm::length(normal);

		localColour = closestRenderable->getMaterial().colourDiffuse;
		localColour *= computeLighting(closestRenderable, normal, position, -_ray.direction);

		float reflectivity = closestRenderable->getMaterial().reflectiveness;
		// handle reflection
		if (_depth <= 0 || reflectivity <= 0.0f)
		{
			return localColour;
		}

		glm::vec3 reflection = reflect(-_ray.direction, normal);
		glm::vec3 reflectedColour = trace(Ray(position, reflection), _depth - 1, Intersect(0.1f, INFINITY));

		// clamp to [0.0:1.0]
		localColour = localColour * (1 - reflectivity) + reflectedColour * reflectivity;

		return localColour;
	}

	float RayTracer::computeLighting(std::shared_ptr<Sphere> _object, glm::vec3 _normal, glm::vec3 _position, glm::vec3 _rayToCamera)
	{
		// the running intensity, will be added to by each light that affects this point
		float intensity = 0;
		float lengthNormal = glm::length(_normal);
		glm::vec3 L;

		float intersectMax;

		for (unsigned int i = 0; i < m_lights.size(); ++i)
		{
			if (m_lights[i].type == LIGHT_POINT)
			{
				L = m_lights[i].position - _position;
				intersectMax = 1.0f;
			}
			else if (m_lights[i].type == LIGHT_DIRECTIONAL)
			{
				L = m_lights[i].direction;
				intersectMax = INFINITY;
			}

			// shadow check
			float closestShadowIntersect = INFINITY;
			std::shared_ptr<Sphere> closestShadowRenderable;
			calculateClosestIntersection(closestShadowRenderable, closestShadowIntersect, Ray(_position, L), Intersect(0.001f, intersectMax));
			if (closestShadowRenderable != NULL)
				continue;

			// diffuse
			float n_dot_l = glm::dot(_normal, L);
			if (n_dot_l > 0)
			{
				intensity += m_lights[i].intensity * n_dot_l / (lengthNormal * glm::length(L));
			}

			// specular
			if (_object->getMaterial().shine != -1)
			{
				glm::vec3 R = 2.0f * _normal * glm::dot(_normal, L) - L;
				float r_dot_v = glm::dot(R, _rayToCamera);
				if (r_dot_v > 0)
				{
					intensity += m_lights[i].intensity * pow(r_dot_v / (length(R) * glm::length(_rayToCamera)), _object->getMaterial().shine);
				}
			}
		}
		intensity = glm::clamp(intensity, 0.0f, 1.0f);
		return intensity;
	}
}