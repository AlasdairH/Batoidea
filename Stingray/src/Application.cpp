// cstdlib
#include <iostream>
#include <memory>

// external libs
#include "SDL/SDL.h"

// program
#include "Logger.h"
#include "Window.h"

#undef main


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