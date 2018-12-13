// cstdlib
#include <iostream>
#include <memory>
#include <vector>

// external libs
#include "SDL/SDL.h"
#include "SDL_Image/SDL_image.h"
#include "GLM/common.hpp"

// program
#include "Logger.h"
#include "Window.h"
#include "ThreadPool.h"
#include "Timer.h"
#include "RayTracer.h"
#include "FileLoader.h"
#include "Object.h"

#undef main

// bat oi de a

#define TICK_INTERVAL 30
static Uint32 next_time;

Uint32 time_left(void)
{
	Uint32 now;

	now = SDL_GetTicks();
	if (next_time <= now)
		return 0;
	else
		return next_time - now;
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
		//return 0;
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		// something went wrong, exit program
		LOG_ERROR("Unable to Initialise SDL_Image: " << IMG_GetError());
		//return 0;
	}

	LOG_MESSAGE("SDL Initialised");

	std::unique_ptr<Window> window = std::make_unique<Window>("Batoidea Raytracer", SCREEN_WIDTH, SCREEN_HEIGHT);

	// set up the settings for rendering
	RayTracerSettings rtSettings = FileLoader::loadSettings("settings.ini");
	// create the ray tracer with the given settings
	RayTracer raytracer(rtSettings);

	Material materialFloor;
	materialFloor.shine = 1000.0f;
	materialFloor.diffuse->load("resources/textures/check.png");
	materialFloor.reflectiveness = 0.5f;
	
	Material materialMK2;
	materialMK2.shine = 500.0f;
	materialMK2.diffuse->load("resources/textures/MK2/base.png");
	materialMK2.reflectiveness = 0.0f;	

	Object object1 = FileLoader::loadObject("resources/models/mk2.obj");
	Object groundPlane = FileLoader::loadObject("resources/models/plane.obj");
	
	object1.setMaterial(materialMK2);
	groundPlane.setMaterial(materialFloor);

	std::vector<Object> renderables;
	renderables.push_back(object1);
	renderables.push_back(groundPlane);

	std::vector<Light> lights;

	Light light1;
	light1.type = LIGHT_POINT;
	light1.direction = glm::vec3(4, 4, 0);
	light1.position = light1.direction;
	light1.intensity = 0.7f;
	lights.push_back(light1);

	Light light2;
	light2.type = LIGHT_POINT;
	light2.direction = glm::vec3(-2, 0, 0);
	light2.position = light2.direction;
	light2.intensity = 0.3f;
	lights.push_back(light2);	
	
	SDL_memset(window->getSurface()->pixels, 0, window->getSurface()->h * window->getSurface()->pitch);

	Timer renderTimer;
	bool renderStarted = false;
	float lastRenderTime = 0.0f;


	bool isRunning = true;
	while (isRunning)
	{
		// START INPUT
		SDL_Event incomingEvent;
		while (SDL_PollEvent(&incomingEvent))
		{
			if (incomingEvent.type == SDL_QUIT)
			{
				raytracer.stopCurrentRender();
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
						renderStarted = true;
						renderTimer.reset();
					}
					else
					{
						LOG_ERROR("Unable to start ray trace, renderer is busy. Consider stopping the current render to start a new one.");
					}
					break;	

				case SDLK_BACKSPACE:
					raytracer.stopCurrentRender();
					break;		

				case SDLK_s:
					if (raytracer.isRenderComplete())
					{
						LOG_MESSAGE("Saving current render");
						SDL_SaveBMP(window->getSurface(), "Render.bmp");
					}
					else
					{
						LOG_ERROR("Unable to save, render in progress");
					}
					break;	

				case SDLK_r:
					LOG_MESSAGE("Reloading Settings");
					raytracer.setSettings(FileLoader::loadSettings("settings.ini"));
					break;				
				
				case SDLK_ESCAPE:
					raytracer.stopCurrentRender();
					isRunning = false;
					break;
				}
			}
		}

		if (renderStarted)
		{
			if (raytracer.isRenderComplete())
			{
				lastRenderTime = renderTimer.getDuration();
				LOG_MESSAGE("Scene rendered in " << lastRenderTime << "s");
				renderStarted = false;
				raytracer.getRenderStatistics().print();
			}
		}

		// END INPUT

		window->render();

		SDL_Delay(time_left());
		next_time += TICK_INTERVAL;
	}

	return 0;
}