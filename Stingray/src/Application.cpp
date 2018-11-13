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

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

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


	Threads::ThreadPool threadPool(std::thread::hardware_concurrency());
	std::vector<std::future<glm::vec3>> futureVector;
	std::vector<glm::vec3> vector;

	/*
	Timer time;
	for (int i = 1; i < 10; ++i)
	{
		futureVector.push_back(
			threadPool.enqueue([=]
			{
				return bounce(100 * i);
			})
		);
	}
	*/

	RayTracerSettings rtSettings;
	RayTracer raytracer(rtSettings);

	std::vector<Sphere> renderables;
	renderables.push_back(Sphere());

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
					LOG_MESSAGE("ENTER");
					raytracer.render(renderables, *window->getSurface());
					break;
				}
			}
		}
		// END INPUT

		/*
		std::vector<int> toRemove;

		for (unsigned int i = 0; i < futureVector.size(); ++i)
		{
			if (futureVector[i].valid())
			{
				if (threadPool.isReady(futureVector[i]))
				{
					LOG_MESSAGE("Thread Complete");
					vector.push_back(futureVector[i].get());
					glm::vec3 colour = vector[vector.size() - 1];
					std::cout << "(" << colour.x << ", " << colour.y << ", " << colour.z << ")" << std::endl;
					std::cout << "Time: " << time.getDuration() << "s" << std::endl;
				}
			}
			else
			{
				toRemove.push_back(i);
			}
		}

		for (int i = 0; i < toRemove.size(); ++i)
		{
			futureVector.erase(futureVector.begin(), futureVector.begin() + i);
		}
		*/

		window->render();
	}

	return 0;
}