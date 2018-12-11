#pragma once

// cstdlib

// external libs
#include <fstream>
#include <sstream>

// program
#include "PCH.h"
#include "Triangle.h"
#include "Object.h"

namespace Batoidea
{
	class ObjectLoader
	{
	public:
		static Object loadObject(const std::string & _filepath);

	protected:
		static std::string loadTextFile(std::string _filepath);

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