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
#include "ObjectLoader.h"
#include "Object.h"

#undef main

// bat oi de a

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


	RayTracerSettings rtSettings;
	//rtSettings.threads = 2;
	RayTracer raytracer(rtSettings);

	Material materialWhite;
	materialWhite.shine = 1000.0f;
	materialWhite.diffuse->setColour(glm::vec3(1.0f, 1.0f, 1.0f));
	materialWhite.reflectiveness = 0.5f;		
	
	Material materialMK2;
	materialMK2.shine = 500.0f;
	materialMK2.diffuse->load("resources/textures/MK2/base.png");
	materialMK2.reflectiveness = 0.1f;

	Object object1 = ObjectLoader::loadObject("resources/models/mk2.obj");
	Object object2 = ObjectLoader::loadObject("resources/models/mk2-1.obj");
	Object groundPlane = ObjectLoader::loadObject("resources/models/plane.obj");
	
	object1.setMaterial(materialMK2);
	object2.setMaterial(materialMK2);
	groundPlane.setMaterial(materialWhite);

	std::vector<Object> renderables;
	renderables.push_back(object1);
	renderables.push_back(object2);
	renderables.push_back(groundPlane);


	std::vector<Light> lights;

	Light light1;
	light1.type = LIGHT_POINT;
	light1.direction = glm::vec3(0, 4, 3);
	light1.position = light1.direction;
	light1.intensity = 0.5f;
	lights.push_back(light1);

	Light light2;
	light2.type = LIGHT_POINT;
	light2.direction = glm::vec3(0, 0, 0);
	light2.position = light2.direction;
	light2.intensity = 0.5f;
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
	}

	return 0;
}