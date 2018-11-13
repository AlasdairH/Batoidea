// cstdlib
#include <iostream>
#include <memory>

// external libs
#include "SDL/SDL.h"

// program
#include "Logger.h"
#include "Window.h"
#include "Sphere.h"
#include "RayTracer.h"

#undef main

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// bat oi de a


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

	SDL_memset(window->getSurface()->pixels, 255, window->getSurface()->h * window->getSurface()->pitch);

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

			if (incomingEvent.type == SDL_KEYDOWN)
			{
				switch (incomingEvent.key.keysym.sym)
				{
				case SDLK_RETURN:
					raytracer.trace(renderables, *window->getSurface());
				}
			}
		}

		window->render();
	}

	return 0;
}