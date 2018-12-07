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
	RayTracer raytracer(rtSettings);

	std::vector<Sphere> renderables;
	Material material;
	material.shine = 1000.0f;

	float distance = 5.0f;
	float radius = 0.8f;

	material.colourDiffuse = glm::vec3(1.0f, 0.0f, 0.0f);

	renderables.push_back(Sphere(glm::vec3(-1.8f, 0, 7), radius, material));
	
	material.colourDiffuse = glm::vec3(0.0f, 1.0f, 0.0f);

	renderables.push_back(Sphere(glm::vec3(0, -1.0f, 5), 1.2f, material));	
	
	material.colourDiffuse = glm::vec3(1.0f, 0.0f, 1.0f);

	renderables.push_back(Sphere(glm::vec3(2.2f, 0, 4), radius + 0.5f, material));

	std::vector<Light> lights;
	Light light;
	light.type = LIGHT_POINT;
	light.direction = glm::vec3(4, -0.5, 0);
	light.position = light.direction;
	//lights.push_back(light);	

	Light light2;
	light2.type = LIGHT_POINT;
	light2.direction = glm::vec3(-3, 2, 0);
	light2.position = light2.direction;
	lights.push_back(light2);

	Light light3;
	light3.type = LIGHT_POINT;
	light3.direction = glm::vec3(3, 2, 0);
	light3.position = light3.direction;
	lights.push_back(light3);
	

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
					raytracer.render(renderables, lights, *window->getSurface());
					break;
				}
			}
		}
		// END INPUT

		window->render();
	}

	return 0;
}