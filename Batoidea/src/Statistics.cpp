#include "Statistics.h"

namespace Batoidea
{
	void Statistics::reset()
	{
		numTriangles = 0;
		numIntersectionTests = 0;
		numIntersections = 0;
		numShadowRays = 0;

		settings = RayTracerSettings();
	}

	void Statistics::print()
	{
		LOG_MESSAGE("Number of Triangles: " << numTriangles);
		LOG_MESSAGE("Number of Intersection Tests: " << numIntersectionTests);
		LOG_MESSAGE("Number of Intersections: " << numIntersections);
		LOG_MESSAGE("Shadow Rays Cast: " << numShadowRays);
		LOG_MESSAGE(" ");
		LOG_MESSAGE("Pixels Rendered: " << settings.renderResolutionWidth * settings.renderResolutionHeight);
		LOG_MESSAGE("Samples per Pixel: " << settings.samplesPerPixel);
		LOG_MESSAGE("Reflection Depth: " << settings.reflectionRecursionDepth);
		LOG_MESSAGE("Threads Used: " << settings.threads);
		LOG_MESSAGE("Render Quads: " << settings.renderQuadResolutionWidth * settings.renderQuadResolutionHeight);
	}

	void Statistics::outputToCSV()
	{
		std::ofstream statsCSV;
		statsCSV.open("example.csv");
		statsCSV << "Stat, value" << "\n";
		statsCSV << "Scene Tris," << numTriangles << "\n";
		statsCSV << "Number of Triangles: " << numTriangles;
		statsCSV << "Number of Intersection Tests: " << numIntersectionTests;
		statsCSV << "Number of Intersections: " << numIntersections;
		statsCSV << "Shadow Rays Cast: " << numShadowRays;
		statsCSV << "Pixels Rendered: " << settings.renderResolutionWidth * settings.renderResolutionHeight;
		statsCSV << "Samples per Pixel: " << settings.samplesPerPixel;
		statsCSV << "Reflection Depth: " << settings.reflectionRecursionDepth;
		statsCSV << "Threads Used: " << settings.threads;
		statsCSV << "Render Quads: " << settings.renderQuadResolutionWidth * settings.renderQuadResolutionHeight;
	}
}