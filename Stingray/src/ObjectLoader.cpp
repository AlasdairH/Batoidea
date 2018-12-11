#include "ObjectLoader.h"

namespace Batoidea
{
	std::vector<Triangle> ObjectLoader::loadObject(const std::string & _filepath)
	{
		std::vector<Triangle> tris;

		std::vector<glm::vec3> verts;

		int parsedLines = 0;
		std::string objFile = ObjectLoader::loadTextFile(_filepath);

		unsigned int cursor = 0;

		for (unsigned int i = 0; i < objFile.size(); ++i)
		{
			// find the index of the next end of line char, starting from i
			unsigned int eol = objFile.find("\n", i);
			// get a substring from i, for end of line - i chars
			std::string line = objFile.substr(i, eol - i);

			// vertex line
			if (line.find("v ") != std::string::npos)
			{
				size_t firstSpaceIndex = line.find(" ", 0);
				size_t secondSpaceIndex = line.find(" ", firstSpaceIndex + 1);
				size_t thirdSpaceIndex = line.find(" ", secondSpaceIndex + 1);
				size_t eolIndex = line.find("\n", i);

				std::string xVal = line.substr(firstSpaceIndex + 1, secondSpaceIndex - firstSpaceIndex - 1);
				std::string yVal = line.substr(secondSpaceIndex + 1, thirdSpaceIndex - secondSpaceIndex - 1);
				std::string zVal = line.substr(thirdSpaceIndex + 1, eolIndex - thirdSpaceIndex - 1);

				float x = std::stof(xVal);
				float y = std::stof(yVal);
				float z = std::stof(zVal);

				verts.push_back(glm::vec3(x, y, z));
				++parsedLines;
			}

			// face line
			if (line.find("f ") != std::string::npos)
			{
				// split the face line
				std::vector<std::string> splitLine = ObjectLoader::split(line, ' ');
				// remove the "f"
				splitLine.erase(splitLine.begin());

				// create a vector for the extracted indices
				std::vector<int> vIncides;
				// for each split in the face components
				for (unsigned int j = 0; j < splitLine.size(); ++j)
				{
					// split the face components by the slash delimiter
					std::vector<std::string> splitBlock = ObjectLoader::split(splitLine[j], '/');
					// get the vertex index
					vIncides.push_back(std::stoi(splitBlock[0]));
				}

				Triangle tri;
				// for the three vertices of a face
				for (unsigned int j = 0; j < 3; ++j)
				{
					// grab the split data for the current vertex and store it together 
					tri.verts[j] = glm::vec3(verts[vIncides[j] - 1]);
				}
				tris.push_back(tri);

				++parsedLines;
			}

			// move the next start point to the end of the last line
			i = eol;
		}


		return tris;
	}

	std::string ObjectLoader::loadTextFile(std::string _filepath)
	{
		std::ifstream inFile;
		//open the input file
		inFile.open(_filepath);

		if ((inFile.rdstate() & std::ifstream::failbit) != 0)
		{
			LOG_ERROR("Failed to Open File: " << _filepath);
		}
		else
		{
			LOG_MESSAGE("Loaded File: " << _filepath);
		}

		std::stringstream strStream;
		// read the file
		strStream << inFile.rdbuf();
		// store the string
		std::string str = strStream.str();

		return str;
	}
}