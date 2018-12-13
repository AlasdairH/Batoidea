#pragma once

#include <iostream>

#define DEBUG

#ifdef DEBUG
#define LOG_MESSAGE(_text) std::cout << "MESSAGE: " << _text << "\n";
#define LOG_WARNING(_text) std::cout << "WARNING: " << _text << std::endl
#define LOG_ERROR(_text) std::cout << "ERROR: " << _text << std::endl
#endif

#define SCREEN_WIDTH 1920	
#define SCREEN_HEIGHT 1080 

#define RAND_FLOAT static_cast <float> (rand()) / static_cast <float> (RAND_MAX)