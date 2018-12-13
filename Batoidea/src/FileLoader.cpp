#include "FileLoader.h"
#include "..\include\FileLoader.h"

namespace Batoidea
{
	Object FileLoader::loadObject(const std::string & _filepath)
	{
		Object object;
		std::vector<Triangle> tris;
		std::vector<glm::vec3> verts;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> tex;

		BoundingBox boundingBox;

		int parsedLines = 0;
		std::string objFile = FileLoader::loadTextFile(_filepath);

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

				// bounding box generation
				// min
				if (x < boundingBox.bounds[0].x)
					boundingBox.bounds[0].x = x;
				if (y < boundingBox.bounds[0].y)
					boundingBox.bounds[0].y = y;
				if (z < boundingBox.bounds[0].z)
					boundingBox.bounds[0].z = z;

				// max
				if (x > boundingBox.bounds[1].x)
					boundingBox.bounds[1].x = x;
				if (y > boundingBox.bounds[1].y)
					boundingBox.bounds[1].y = y;
				if (z > boundingBox.bounds[1].z)
					boundingBox.bounds[1].z = z;

				verts.push_back(glm::vec3(x, y, z));
				++parsedLines;
			}			
			
			// normal line
			if (line.find("n ") != std::string::npos)
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

				normals.push_back(glm::vec3(x, y, z));
				++parsedLines;
			}

			// texture coord line
			if (line.find("vt ") != std::string::npos)
			{
				size_t firstSpaceIndex = line.find(" ", 0);
				size_t secondSpaceIndex = line.find(" ", firstSpaceIndex + 1);
				size_t eolIndex = line.find("\n", i);

				std::string xVal = line.substr(firstSpaceIndex + 1, secondSpaceIndex - firstSpaceIndex - 1);
				std::string yVal = line.substr(secondSpaceIndex + 1, eolIndex - secondSpaceIndex - 1);

				tex.push_back(glm::vec2(std::stof(xVal), std::stof(yVal)));
				++parsedLines;
			}

			// face line
			if (line.find("f ") != std::string::npos)
			{
				// split the face line
				std::vector<std::string> splitLine = FileLoader::split(line, ' ');
				// remove the "f"
				splitLine.erase(splitLine.begin());

				// create a vector for the extracted indices
				std::vector<int> vIncides;
				std::vector<int> vtIncides;
				std::vector<int> vnIncides;
				// for each split in the face components
				for (unsigned int j = 0; j < splitLine.size(); ++j)
				{
					// split the face components by the slash delimiter
					std::vector<std::string> splitBlock = FileLoader::split(splitLine[j], '/');
					// get the vertex index
					vIncides.push_back(std::stoi(splitBlock[0]));
					// get the vertex texture index
					vtIncides.push_back(std::stoi(splitBlock[1]));
					// get the vertex normal index
					vnIncides.push_back(std::stoi(splitBlock[2]));
				}

				Triangle tri;
				// for the three vertices of a face
				for (unsigned int j = 0; j < 3; ++j)
				{
					// grab the split data for the current vertex and store it together 
					tri.verts[j] = glm::vec3(verts[vIncides[j] - 1]);
					tri.texture[j] = glm::vec2(tex[vtIncides[j] - 1]);
					tri.normals[j] = glm::vec3(normals[vnIncides[j] - 1]);
				}
				tris.push_back(tri);

				++parsedLines;
			}

			// move the next start point to the end of the last line
			i = eol;
		}

		object.setBoundingBox(boundingBox);
		object.tris = tris;

		return object;
	}

	RayTracerSettings FileLoader::loadSettings(const std::string & _filepath)
	{
		RayTracerSettings settings;
		std::string objFile = FileLoader::loadTextFile(_filepath);

		unsigned int cursor = 0;

		for (unsigned int i = 0; i < objFile.size(); ++i)
		{
			// find the index of the next end of line char, starting from i
			unsigned int eol = objFile.find("\n", i);
			// get a substring from i, for end of line - i chars
			std::string line = objFile.substr(i, eol - i);

			if (line.find("renderResolutionWidth") != std::string::npos)
			{
				std::vector<std::string> splitLine = FileLoader::split(line, ' ');
				settings.renderResolutionWidth = std::stoi(splitLine[1]);
			}
			else if (line.find("renderResolutionHeight") != std::string::npos)
			{
				std::vector<std::string> splitLine = FileLoader::split(line, ' ');
				settings.renderResolutionHeight = std::stoi(splitLine[1]);
			}
			else if (line.find("threads") != std::string::npos)
			{
				std::vector<std::string> splitLine = FileLoader::split(line, ' ');
				if (splitLine[1] == "all")
				{
					settings.threads = std::thread::hardware_concurrency();
				}
				else
				{
					settings.threads = std::stoi(splitLine[1]);
				}
			}
			else if (line.find("renderQuadResolutionWidth") != std::string::npos)
			{
				std::vector<std::string> splitLine = FileLoader::split(line, ' ');
				settings.renderQuadResolutionWidth = std::stoi(splitLine[1]);
			}
			else if (line.find("renderQuadResolutionHeight") != std::string::npos)
			{
				std::vector<std::string> splitLine = FileLoader::split(line, ' ');
				settings.renderQuadResolutionHeight = std::stoi(splitLine[1]);
			}
			else if (line.find("reflectionRecursionDepth") != std::string::npos)
			{
				std::vector<std::string> splitLine = FileLoader::split(line, ' ');
				settings.reflectionRecursionDepth = std::stoi(splitLine[1]);
			}
			else if (line.find("samplesPerPixel") != std::string::npos)
			{
				std::vector<std::string> splitLine = FileLoader::split(line, ' ');
				settings.samplesPerPixel = std::stoi(splitLine[1]);
			}
			else if (line.find("renderShadows") != std::string::npos)
			{
				std::vector<std::string> splitLine = FileLoader::split(line, ' ');
				settings.renderShadows = std::stoi(splitLine[1]);
			}
			else if (line.find("backgroundColour") != std::string::npos)
			{
				std::vector<std::string> splitLine = FileLoader::split(line, ' ');
				settings.backgroundColour.r = std::stof(splitLine[1]);
				settings.backgroundColour.g = std::stof(splitLine[2]);
				settings.backgroundColour.b = std::stof(splitLine[3]);
			}

			i = eol;
		}
		return settings;
	}

	std::string FileLoader::loadTextFile(std::string _filepath)
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