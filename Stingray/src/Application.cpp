// cstdlib
#include <iostream>
#include <memory>
#include <math.h>

// external libs
#include "SDL/SDL.h"
#include "GLM/common.hpp"

// program
#include "Logger.h"
#include "Window.h"
#include "ThreadPool.h"
#include "Timer.h"

#undef main

glm::vec3 bounce()
{
	int iterations = 5000;

	glm::vec3 result = glm::vec3(0);

	for (float x = 0.0f; x < 1.0f; x += 0.01f)
	{
		for (float y = 1.0f; y > 0.0f; y -= 0.01f)
		{
			for (int i = 0; i < iterations; ++i)
			{
				result.x = sin(cos(cos(x + y)));
				result.y = cos(sin(cos(x + y)));
				result.z = cos(cos(sin(x + y)));
			}
		}
	}

	return result;
}


int main()
{
	using namespace Stingray;

	LOG_MESSAGE("Initialising SDL");
	// attempt to initialise opengl
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		// something went wrong, exit program
		LOG_ERROR("Unable to Initialise SDL");
	}

	LOG_MESSAGE("SDL Initialised");

	std::unique_ptr<Window> window = std::make_unique<Window>("Stingray Ray Tracer", 800,  600);

	Threads::ThreadPool threadPool(std::thread::hardware_concurrency());

	Timer time;
	auto threadResult = threadPool.enqueue([] 
	{
		return bounce();
	});

	glm::vec3 colour = threadResult.get();

	//std::cout << "(" << colour.x << ", " << colour.y << ", " << colour.z << ")" << std::endl;
	std::cout << "Time: " << time.getDuration() << "s" << std::endl;

	bool isRunning = true;
	while (isRunning)
	{
		SDL_Event incomingEvent;
		while (SDL_PollEvent(&incomingEvent))
		{
			if (incomingEvent.type == SDL_QUIT)
			{
				isRunning = false;
			}
		}
	}

	return 0;
}