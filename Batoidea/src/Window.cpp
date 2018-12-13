#include "Window.h"

namespace Batoidea
{
	Window::Window(const std::string _title, const int _width, const int _height)
	{
		m_window = SDL_CreateWindow(_title.c_str(),
			50, 50,
			_width, _height,
			SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);

		m_renderer = SDL_CreateRenderer(m_window, -1, 0);

		resize(_width, _height);
	}

	Window::~Window()
	{
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
	}

	void Window::render()
	{
		SDL_UpdateWindowSurface(m_window);
	}

	void Window::resize(int _width, int _height)
	{
		m_width = _width;
		m_height = _height;
	}

	void Window::setTitle(const std::string & _title)
	{
		SDL_SetWindowTitle(m_window, _title.c_str());
	}


}