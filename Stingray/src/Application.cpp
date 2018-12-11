// cstdlib
#include <iostream>
#include <memory>
#include <vector>

// external libs
#include "SDL/SDL.h"
#include "GLM/common.hpp"

// program
#include "Logger.h"
#include "Window.h"
#include "Sphere.h"
#include "ThreadPool.h"
#include "Timer.h"
#include "RayTracer.h"

#define RAND_FLOAT static_cast <float> (rand()) / static_cast <float> (RAND_MAX)

#undef main

// bat oi de a

glm::vec3 bounce(int _iterations)
{
	glm::vec3 result = glm::vec3(0);

	for (float x = 0.0f; x < 1.0f; x += 0.01f)
	{
		for (float y = 1.0f; y > 0.0f; y -= 0.01f)
		{
			for (int i = 0; i < _iterations; ++i)
			{
				result.x = sin(cos(cos(x + y)));
				result.y = cos(sin(cos(x + y)));
				result.z = cos(cos(sin(x + y)));
			}
		}
	}

	return result;
}

void store(std::vector<glm::vec3> &_vector, glm::vec3 _value)
{
	_vector.push_back(_value);
}

int main()
{
	using namespace Batoidea;

	LOG_MESSAGE("Initialising SDL");
	// attempt to initialise opengl
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		// something went wrong, exit program
		LOG_ERROR("Unable to Initialise SDL");
	}

	LOG_MESSAGE("SDL Initialised");

	std::unique_ptr<Window> window = std::make_unique<Window>("Batoidea Raytracer", SCREEN_WIDTH, SCREEN_HEIGHT);


	RayTracerSettings rtSettings;
	rtSettings.threads = std::thread::hardware_concurrency();
	rtSettings.threads = 8;
	RayTracer raytracer(rtSettings);

	std::vector<Sphere> renderables;
	Material material;
	material.shine = 1000.0f;

	float distance = 5.0f;
	float radius = 0.3f;

	for (int x = -3; x <= 3; ++x)
	{
		for (int y = -3; y <= 3; ++y)
		{
			for (int z = 8; z <= 14; ++z)
			{
				material.colourDiffuse = glm::vec3(RAND_FLOAT, RAND_FLOAT, RAND_FLOAT);
				renderables.push_back(Sphere(glm::vec3(x, y, z), radius, material));
			}
		}
	}

	std::vector<Light> lights;

	Light light1;
	light1.type = LIGHT_POINT;
	light1.direction = glm::vec3(-4, 0, 0);
	light1.position = light1.direction;
	light1.intensity = 0.5f;
	lights.push_back(light1);

	Light light2;
	light2.type = LIGHT_POINT;
	light2.direction = glm::vec3(4, 0, 0);
	light2.position = light2.direction;
	light2.intensity = 0.5f;
	lights.push_back(light2);
	

	SDL_memset(window->getSurface()->pixels, 0, window->getSurface()->h * window->getSurface()->pitch);

	bool isRunning = true;
	while (isRunning)
	{
		// START INPUT
		SDL_Event incomingEvent;
		while (SDL_PollEvent(&incomingEvent))
		{
			if (incomingEvent.type == SDL_QUIT)
			{
				isRunning = false;
			}

			if (incomingEvent.type == SDL_KEYDOWN)
			{
				switch (incomingEvent.key.keysym.sym)
				{
				case SDLK_RETURN:
					if (raytracer.isRenderComplete())
					{
						SDL_FillRect(window->getSurface(), NULL, 0x000000);
						raytracer.render(renderables, lights, *window->getSurface());
					}
					else
					{
						LOG_ERROR("Unable to start ray trace, renderer is busy. Consider stopping the current render to start a new one.");
					}
					break;				
				case SDLK_BACKSPACE:
					raytracer.stop();
					break;
				}
			}
		}
		// END INPUT

		window->render();
	}

	return 0;
}