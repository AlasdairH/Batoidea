#pragma once

// cstdlib

// external libs

// program
#include "PCH.h"

namespace Batoidea
{
	/*! @class Texture
	*	@brief A texture, usually applied to a material
	*
	*	This texture class is usually applied to a material to be used in rendering.
	*/
	class Texture
	{
	public:
		/** @brief Texture Constructor
		*
		*	Creates a texture with the given size
		*/
		Texture(const int _width, const int _height);
		/** @brief Triangle Deconstructor
		*
		*	Frees the texture
		*/
		~Texture();
		
		/** @brief Loads an image to a texture
		*	@param _filepath the path to the texture
		*
		*	Loads an image to the texture. Supports PNG.
		*/
		void load(std::string _filepath);

		/** @brief Sets the texture to a block colour
		*	@param _colour The colour to set
		*
		*	Sets the texture to the provided block colour
		*/
		void setColour(glm::vec3 _colour);

		/** @brief Gets the texture width
		*	@return The texture width
		*
		*	Returns the texture width
		*/
		int getWidth() { return m_texture->w; }
		/** @brief Gets the texture height
		*	@return The texture height
		*
		*	Returns the texture height
		*/
		int getHeight() { return m_texture->h; }

		/** @brief Samples the texture at position 0, 0
		*	@return The colour at position 0, 0
		*
		*	Calls sample on position 0, 0
		*/
		glm::vec3 sample();
		/** @brief Samples the texture at position x, y
		*	@return _x the pixel x coordinate 
		*	@return _y the pixel y coordinate 
		*	@return The colour at position x, y in RGB
		*
		*	Samples the texture for the colour at the specified position
		*/
		glm::vec3 sample(const int _x, const int _y);

	protected:
		SDL_Surface *m_texture;	/**< The SDL_Surface texture */
		//void *m_pixels;
	};
}