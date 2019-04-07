#pragma once
#include <omp.h>

#include "stdafx.h"

#include "Ray.h"
#include "CastedRay.h"

#include "Camera.h"
#include "PPM.h"

#include "pdf.h"

#include "Stats.h"
#include "Random.h"

#include "Scenes.h"
#include <iostream>
#include <limits>
#include <vector>
#include <memory>
#include <chrono>
#include <ctime>  
#include <algorithm>

const float PI = 3.14159265359f;


// This function reset the performance statistique variable
void resetStat()
{
	numberOfGeometry = 0;
	numberOfPrimaryRay = 0;
	numberOfRaySphereTest = 0;
	numberOfRaySphereIntersection = 0;
}

inline glm::vec3 deNaN(const glm::vec3 &c)
{
	glm::vec3 temp = c;
	if (!(temp[0] == temp[0]))
	{
		temp[0] = 0.0f;
	}
	if (!(temp[1] == temp[1]))
	{
		temp[1] = 0.0f;
	}
	if (!(temp[2] == temp[2]))
	{
		temp[2] = 0.0f;
	}
	return temp;
}

void printStat()
{
	std::cout << "Total number of geometry : "  << numberOfGeometry << std::endl;
	std::cout << "Total number of primary ray : " << numberOfPrimaryRay << std::endl;
	std::cout << "Total number of ray-spheres tests : " << numberOfRaySphereTest << std::endl;
	std::cout << "Total number of ray-spheres intersections : " << numberOfRaySphereIntersection << std::endl;
}

// TODO : Check to change this pointer for a smartPointer ?
glm::vec3 color(CastedRay& r, std::unique_ptr<Hitable> &world, int depth)
{
	glm::vec3 col(0.0f);
	if (world->intersect(r, 0.001f, std::numeric_limits<float>::max()))
	{
		if (depth < 50 && r.hitRec().matPtr != nullptr)
		{
			CastedRay scattered;
			glm::vec3 emitted = r.hitRec().matPtr->emitted(r, r.hitRec().u, r.hitRec().v, r.hitRec().P);
			float pdf_val;

			if (r.hitRec().matPtr->scatter(r, col, scattered, pdf_val))
			{
				CosinePDF p1(r.hitRec().N);
				std::shared_ptr<Hitable> lightShape = std::make_shared<RectXZ>(213.0f, 343.0f, 227.0f, 332.0f, 554.0f, nullptr);
				HitablePDF p2(lightShape, r.hitRec().P);
				MixturePDF p(std::make_shared<CosinePDF>(p1), std::make_shared<HitablePDF>(p2));
				scattered = CastedRay(r.hitRec().P, p.generate(), r.time());
				pdf_val = p.value(scattered.direction());
				col = emitted + col * r.hitRec().matPtr->scattering_pdf(r,scattered) * color(scattered, world, depth + 1) / pdf_val;
			}
			else
			{
				col = emitted;
			}
		}
		else
		{
			return glm::vec3(0.0f);
		}
	}
	else
	{
		/*glm::vec3 unit_direction = glm::normalize(r.direction());
		float t = 0.5f * (unit_direction.y + 1.0f);
		col =  glm::vec3(1.0f) *(1.0f - t) + glm::vec3(0.5f, 0.7f, 1.0f) * t;*/
	}
	return col;
}

int main() {

	resetStat();

	// Set size
	int width = 500;
	int height = 500;
	int loopAA = 500;

	// TODO : find a better way to do this
	numberOfPrimaryRay = width * height * loopAA;

	PPM image(width, height);

	//std::unique_ptr<Hitable>  world = std::make_unique<BVHNode>(list);
	//std::unique_ptr<Hitable>  world(finalRandomScene());
	//std::unique_ptr<Hitable>  world(twoSphere());
	//std::unique_ptr<Hitable>  world(twoPerlinSphere());
	//std::unique_ptr<Hitable>  world(earth());
	//std::unique_ptr<Hitable>  world(simpleLight());
	std::unique_ptr<Hitable>  world(cornellBox());
	//std::unique_ptr<Hitable>  world(finalRandomSceneNextWeek());

	// Camera information
	glm::vec3 origin(278.0f, 278.0f, -800.0f);
	glm::vec3 lookat(278.0f, 278.0f, 0.0f);
	float distToFocus = 10.0f;
	float aperture = 0.0f;
	float vfov = 40.0f;

	Camera cam(origin, lookat, glm::vec3(0.0, 1.0, 0.0), vfov, float(width) / float(height), aperture, distToFocus, 0.0f, 1.0f);

	// Start time of the rendering
	auto start = std::chrono::system_clock::now();

#pragma omp parallel for schedule(dynamic) num_threads(4)
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{

			glm::vec3 col(0.0f);
			for (int s = 0; s < loopAA; s++)
			{
				float u = float(i + dis(gen)) / float(width);
				float v = float(height - j + dis(gen)) / float(height);

				CastedRay r = cam.generateRay(u, v);
				col += deNaN(color(r, world, 0));
			}

			col /= float(loopAA);
			col = glm::vec3(sqrt(col.x), sqrt(col.y), sqrt(col.z));

			int red = int(255.99 * col.x);
			int green = int(255.99 * col.y);
			int blue = int(255.99 * col.z);
			glm::ivec3 result = glm::ivec3(red, green, blue);

			image[j][i] = result;
		}
	}

	// End time of the rendering
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	std::cout << "elapsed time: " << elapsed_seconds.count() << std::endl;
	printStat();

	image.write("test");

	std::cin.get();

}