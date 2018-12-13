#pragma once

// cstdlib

// external libs
#include <fstream>
#include <sstream>

// program
#include "PCH.h"
#include "Triangle.h"
#include "Object.h"
#include "RayTracerSettings.h"

namespace Batoidea
{
	/*! @class ObjectLoader
	*	@brief Loads vaious file types
	*
	*	Loads files such as .obj and .ini
	*/
	class FileLoader
	{
	public:
		/** @brief Loads an object from a .obj file
		*	@param _filepath The path to the file to load
		*	@return The object loaded from file.
		*
		*	Loads an object from a .obj file. This code is a modification of the CookieEngine object
		*	loader, changed to load directly to triangles.
		*/
		static Object loadObject(const std::string &_filepath);		
		
		/** @brief Loads settings from a .ini file
		*	@param _filepath The path to the file to load
		*	@return The loaded settings
		*
		*	Loads settings from a .ini file.
		*/
		static RayTracerSettings loadSettings(const std::string &_filepath);

	protected:
		/** @brief Loads text from a file to a string
		*	@param _filepath The path to the file to load
		*	@return A string containing the text from the file
		*
		*	Loads a text based file (.txt / .obj / etc) to a string
		*/
		static std::string loadTextFile(std::string _filepath);

		/** @brief Splits a string based on a delimiter
		*	@param _s The string to split
		*	@param _delimiter The delimiter to split the string around
		*	@return A vector of strings, split by the delimiter
		*
		*	Splits a string around a delimiter. Used in parsing files.
		*/
		static std::vector<std::string> split(const std::string &_s, char _delimiter)
		{
			std::vector<std::string> tokens;
			std::string token;
			std::istringstream tokenStream(_s);
			while (std::getline(tokenStream, token, _delimiter))
			{
				tokens.push_back(token);
			}
			return tokens;
		}

	};
}