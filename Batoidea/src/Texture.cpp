#include "Texture.h"

namespace Batoidea
{
	Texture::Texture(const int _width, const int _height)
	{
		m_texture = SDL_CreateRGBSurface(0, _width, _height, 32, 0, 0, 0, 0);
		setColour(glm::vec3(255, 0, 144));
	}

	Texture::~Texture()
	{
		SDL_FreeSurface(m_texture);
	}

	void Texture::load(std::string _filepath)
	{
		m_texture = IMG_Load(_filepath.c_str());
		if (!m_texture)
		{
			LOG_ERROR("Unable to load image " << _filepath << "! SDL_image Error: " << IMG_GetError());
			return;
		}
	}

	void Texture::setColour(glm::vec3 _colour)
	{
		SDL_FillRect(m_texture, NULL, SDL_MapRGB(m_texture->format, _colour.r, _colour.g, _colour.b));
	}

	glm::vec3 Texture::sample()
	{
		return sample(0, 0);
	}

	glm::vec3 Texture::sample(const int _x, const int _y)
	{
		SDL_Color sdlColour;
		Uint32 col = 0;
		SDL_LockSurface(m_texture);
		//determine position
		char* pPosition = (char*)m_texture->pixels;

		//offset by y
		pPosition += (m_texture->pitch * _y);

		//offset by x
		pPosition += (m_texture->format->BytesPerPixel * _x);

		//copy pixel data
		memcpy(&col, pPosition, m_texture->format->BytesPerPixel);

		//convert color
		SDL_GetRGB(col, m_texture->format, &sdlColour.r, &sdlColour.g, &sdlColour.b);

		glm::vec3 colour = glm::vec3(sdlColour.r, sdlColour.g, sdlColour.b) / 255.0f;

		SDL_UnlockSurface(m_texture);
		return (colour);
	}	
}