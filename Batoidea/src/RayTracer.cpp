#include "RayTracer.h"

namespace Batoidea
{
	RayTracer::RayTracer(const RayTracerSettings _settings)
	{
		m_settings = _settings;

		m_threadPool = std::make_shared<Threads::ThreadPool>(m_settings.threads);
		
		m_camera = std::make_shared<Camera>(m_settings.renderResolutionWidth, m_settings.renderResolutionHeight, glm::vec3(0, -0.1f, 0));
	}

	SDL_Surface RayTracer::render(std::vector<Object> &_renderables, std::vector<Light> &_lights, SDL_Surface &_surface)
	{
		m_statistics.reset();
		m_statistics.settings = m_settings;


		LOG_MESSAGE("Calculating Render Quads");
		std::vector<RenderQuad> renderQuads = calculateRenderQuads();

		// transfer objects and lights to raytracer for ease of multi-threaded access
		m_objects = _renderables;
		m_lights = _lights;
		m_pixels = (Uint32*)_surface.pixels;
		
		// count number of triangles in scene
		for (unsigned int i = 0; i < m_objects.size(); ++i)
		{
			m_statistics.numTriangles += (int)m_objects[i].tris.size();
		}

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

	glm::vec3 RayTracer::trace(const Ray &_ray, int _depth, Intersect _limits)
	{
		std::shared_ptr<Object> closestRenderable;
		// grab the closest intersection for the ray
		Intersect intersect = calculateClosestIntersection(closestRenderable, _ray, _limits);
		intersect.normal = glm::normalize(intersect.normal);

		// the background colour
		glm::vec3 localColour = m_settings.backgroundColour;

		// if there is no renderable along the ray
		if (closestRenderable == nullptr)
		{
			return localColour;
		}
		// if we got to here then there was an intersection
		++m_statistics.numIntersections;

		glm::vec3 position = _ray.point_at(intersect.t1);

		Material rennderableMaterial = closestRenderable->getMaterial();

		localColour = rennderableMaterial.diffuse->sample(intersect.texCoord.x * (rennderableMaterial.diffuse->getWidth() - 1), rennderableMaterial.diffuse->getHeight() - (intersect.texCoord.y * (rennderableMaterial.diffuse->getHeight() - 1)));
		localColour *= computeLighting(closestRenderable, intersect.normal, position, -_ray.direction);

		// handle reflection
		float reflectivity = closestRenderable->getMaterial().reflectiveness;
		if (_depth <= 0 || reflectivity <= 0.0f)
		{
			return localColour;
		}

		glm::vec3 reflection = reflect(-_ray.direction, intersect.normal);
		glm::vec3 reflectedColour = trace(Ray(position, reflection), _depth - 1, Intersect(0.001f, INFINITY, glm::vec3(0)));

		localColour = localColour * (1 - reflectivity) + reflectedColour * reflectivity;

		return localColour;
	}

	Intersect RayTracer::calculateClosestIntersection(std::shared_ptr<Object> &_renderable, Ray _ray, Intersect _limits)
	{
		Intersect rtnIntersect;
		float closestIntersect = INFINITY;
		std::shared_ptr<Object> closestRenderable = nullptr;
		
		// get the closest objcet in the ray intersections
		for (unsigned int i = 0; i < m_objects.size(); ++i)
		{
			// test for a bounding intersection before testing all triangles of a mesh
			++m_statistics.numIntersectionTests;

			if (!m_objects[i].boundIntersection(_ray))
			{
				continue;
			}

			// loop through each triangle of the object and check it for an intersection with the ray
			for (unsigned int j = 0; j < m_objects[i].tris.size(); ++j)
			{
				// get the intersect
				Intersect intersect = m_objects[i].tris[j].intersect(Ray(_ray.origin, _ray.direction), _limits);

				++m_statistics.numIntersectionTests;


				if (intersect.t1 > _limits.t1 && intersect.t1 < _limits.t2 && intersect.t1 < rtnIntersect.t1)
				{
					rtnIntersect.t1 = intersect.t1;
					rtnIntersect.t2 = intersect.t1;
					closestRenderable = std::make_shared<Object>(m_objects[i]);
					rtnIntersect.normal = intersect.normal;
					rtnIntersect.texCoord = intersect.texCoord;
				}
				if (intersect.t2 > _limits.t1 && intersect.t2 < _limits.t2 && intersect.t2 < rtnIntersect.t1)
				{
					rtnIntersect.t1 = intersect.t2;
					rtnIntersect.t2 = intersect.t2;
					closestRenderable = std::make_shared<Object>(m_objects[i]);
					rtnIntersect.normal = intersect.normal;
					rtnIntersect.texCoord = intersect.texCoord;
				}
			}
		}

		_renderable = closestRenderable;

		return rtnIntersect;
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
		// loop through the quad
		for (int y = (int)_start.y; y < (int)_finish.y; ++y)
		{
			for (int x = (int)_start.x; x < (int)_finish.x; ++x)
			{
				glm::vec3 pixelColour;
				
				for (int i = 0; i < m_settings.samplesPerPixel; ++i)
				{
					float sampleVarianceX = 0;
					float sampleVarianceY = 0;
					// if there is more than 1 pixel sample then add some variance to the sample location
					if (m_settings.samplesPerPixel > 1)
					{
						sampleVarianceX = RAND_FLOAT;
						sampleVarianceY = RAND_FLOAT;
					}

					Ray cameraRay = m_camera->getRay((float)x + sampleVarianceX, (float)y + sampleVarianceY);
					pixelColour += trace(cameraRay, m_settings.reflectionRecursionDepth, Intersect(0.0f, 100.0f, glm::vec3(0)));
				}

				pixelColour /= (float)m_settings.samplesPerPixel;
				pixelColour *= 255;
				m_pixels[x + m_settings.renderResolutionWidth * y] = ((int)pixelColour.r << 16) | ((int)pixelColour.g << 8) | (int)pixelColour.b;
			}
		}
	}

	float RayTracer::computeLighting(std::shared_ptr<Object> _renderable, glm::vec3 _normal, glm::vec3 _position, glm::vec3 _rayToCamera)
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

			L = glm::normalize(L);
			
			// shadow check
			if (m_settings.renderShadows)
			{
				std::shared_ptr<Object> closestShadowRenderable;
				Intersect shadowIntersect = calculateClosestIntersection(closestShadowRenderable, Ray(_position, L), Intersect(0.001f, INFINITY));
				++m_statistics.numShadowRays;

				
				if (closestShadowRenderable != NULL)
					continue;
			}

			// diffuse
			float n_dot_l = glm::dot(_normal, L);
			if (n_dot_l > 0)
			{
				intensity += m_lights[i].intensity * n_dot_l / (lengthNormal * glm::length(L));
			}

			// specular
			if (_renderable->getMaterial().shine != -1)
			{
				glm::vec3 R = 2.0f * _normal * glm::dot(_normal, L) - L;
				float r_dot_v = glm::dot(R, _rayToCamera);
				if (r_dot_v > 0)
				{
					intensity += m_lights[i].intensity * pow(r_dot_v / (length(R) * glm::length(_rayToCamera)), _renderable->getMaterial().shine);
				}
			}
		}
		intensity = glm::clamp(intensity, 0.0f, 1.0f);
		return intensity;
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
}