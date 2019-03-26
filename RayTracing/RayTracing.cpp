#pragma once



#include <omp.h>

#include "stdafx.h"

#include "Ray.h"
#include "CastedRay.h"

#include "Camera.h"
#include "PPM.h"

#include "HitableList.h"
#include "BVHNode.h"

#include "Sphere.h"
#include "MovingSphere.h"
#include "Rectangle.h"
#include "FlipNormal.h"
#include "Box.h"
#include "ConstantMedium.h"

#include "Rotation.h"
#include "Translate.h"

#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"

#include "ConstantTexture.h"
#include "CheckerTexture.h"
#include "NoiseTexture.h"
#include "ImageTexture.h"
#include "DiffuseLight.h"

#include "Stats.h"
#include "Random.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <limits>
#include <vector>
#include <memory>
#include <chrono>
#include <ctime>  
#include <algorithm>

const float PI = 3.14159265359f;

std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<> dis(0.0f, 1.0f);

// This function reset the performance statistique variable
void resetStat()
{
	numberOfGeometry = 0;
	numberOfPrimaryRay = 0;
	numberOfRaySphereTest = 0;
	numberOfRaySphereIntersection = 0;
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
		CastedRay scattered;
		if (depth < 50 && r.hitRec().matPtr != nullptr)
		{
			glm::vec3 emitted = r.hitRec().matPtr->emitted(r.hitRec().u, r.hitRec().v, r.hitRec().P);
			if (r.hitRec().matPtr->scatter(r, col, scattered))
			{
				col = emitted + col * color(scattered, world, depth + 1);
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


std::unique_ptr<Hitable> basicScene()
{
	std::vector<std::shared_ptr<Hitable>> list;
	list.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.8f, 0.3f, 0.3f)))));
	list.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, std::make_shared<Metal>(glm::vec3(0.8f, 0.8f, 0.0f), 0.3f)));
	list.push_back(std::make_shared<Sphere>(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 0.3f)));
	list.push_back(std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f, std::make_shared<Dielectric>(1.5f)));
	list.push_back(std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, -1.0f), -0.45f, std::make_shared<Dielectric>(1.5f)));

	return std::make_unique<BVHNode>(list);
}

std::unique_ptr<Hitable> finalRandomScene()
{
	std::vector<std::shared_ptr<Hitable> > list;

	auto checker = std::make_shared<CheckerTexture>(std::make_shared<ConstantTexture>(glm::vec3(0.2f, 0.3f, 0.1f)), std::make_shared<ConstantTexture>(glm::vec3(0.9f, 0.9f, 0.9f)));
	list.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, -1000.0f, 0.0f), 1000, std::make_shared<Lambertian>(checker)));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float chooseMat = float(dis(gen));
			glm::vec3 center(a + 0.9f * dis(gen), 0.2, b + 0.9f * dis(gen));
			if (glm::length(center - glm::vec3(4.0f, 0.2f, 0.0f)) > 0.9f)
			{
				if (chooseMat < 0.8f) // Diffuse material
				{
					//list.push_back(std::make_shared<Sphere>(center, 0.2f, std::make_shared<Lambertian>(glm::vec3(dis(gen) * dis(gen), dis(gen) * dis(gen), dis(gen) * dis(gen)))));
					// TO REMOVE
					list.push_back(std::make_shared<Sphere>(center, 0.2f, 
						std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(dis(gen) * dis(gen), dis(gen) * dis(gen), dis(gen) * dis(gen))))));
				}
				else if (chooseMat < 0.95f) // Metal material
				{
					list.push_back(std::make_shared<Sphere>(center, 0.2f, 
						std::make_shared<Metal>(glm::vec3(0.5f * (1.0f + dis(gen)), 0.5f * (1.0f + dis(gen)), 0.5f * (1.0f + dis(gen))), 0.5f * dis(gen))));
				}
				else // Glass material
				{
					list.push_back(std::make_shared<Sphere>(center, 0.2f, std::make_shared<Dielectric>(1.5f)));
				}
			}
		}
	}

	list.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, std::make_shared<Dielectric>(1.5f)));
	list.push_back(std::make_shared<Sphere>(glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f, std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.4f, 0.2f, 0.1f)))));
	list.push_back(std::make_shared<Sphere>(glm::vec3(4.0, 1.0f, 0.0f), 1.0f, std::make_shared<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.0f)));

	// TODO : find a better way to do this
	numberOfGeometry = list.size();

	return std::make_unique<BVHNode>(list);
}

std::unique_ptr<Hitable> twoSphere()
{
	auto checker = std::make_shared<CheckerTexture>(std::make_shared<ConstantTexture>(glm::vec3(0.2f, 0.3f, 0.1f)), std::make_shared<ConstantTexture>(glm::vec3(0.9f, 0.9f, 0.9f)));

	std::vector<std::shared_ptr<Hitable> > list;

	list.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, -10.0f, 0.0f), 10, std::make_shared<Lambertian>(checker)));
	list.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 10.0f, 0.0f), 10, std::make_shared<Lambertian>(checker)));

	return std::make_unique<BVHNode>(list);
}

std::unique_ptr<Hitable> twoPerlinSphere()
{
	auto perlinTex = std::make_shared<NoiseTexture>(3);

	std::vector<std::shared_ptr<Hitable> > list;

	list.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f, std::make_shared<Lambertian>(perlinTex)));
	list.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 2.0f, 0.0f), 2.0f, std::make_shared<Lambertian>(perlinTex)));

	return std::make_unique<BVHNode>(list);
}

std::unique_ptr<Hitable> earth()
{

	int widthTex, heightTex, chanelTex;
	unsigned char *tex_data = stbi_load("earthNight.jpg", &widthTex, &heightTex, &chanelTex, 0);
	auto imgTex = std::make_shared<ImageTexture>(tex_data, widthTex, heightTex);

	std::vector<std::shared_ptr<Hitable> > list;

	list.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, 0.0f), 2.0f, std::make_shared<Lambertian>(imgTex)));

	return std::make_unique<BVHNode>(list);
}

std::unique_ptr<Hitable> simpleLight()
{

	auto perlinTex = std::make_shared<NoiseTexture>(4);

	std::vector<std::shared_ptr<Hitable> > list;

	list.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f, std::make_shared<Lambertian>(perlinTex)));
	list.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 2.0f, 0.0f), 2.0f, std::make_shared<Lambertian>(perlinTex)));
	list.push_back(std::make_shared<RectXY>(3.0f, 5.0f, 1.0f, 3.0f, -2.0, std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(glm::vec3(4.0f, 4.0f, 4.0f)))));

	return std::make_unique<BVHNode>(list);
}

std::unique_ptr<Hitable> cornellBox()
{

	auto red = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.65f, 0.05f, 0.05f)));
	auto white = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.73f, 0.73f, 0.73f)));
	auto green = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.12f, 0.45f, 0.15f)));
	auto light = std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(glm::vec3(15.0f, 15.0f, 15.0f)));

	std::vector<std::shared_ptr<Hitable> > list;

	list.push_back(std::make_shared<FlipNormal>(std::make_shared<RectYZ>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, green )));
	list.push_back(std::make_shared<RectYZ>(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, red));
	list.push_back(std::make_shared<RectXZ>(213.0f, 343.0f, 227.0f, 332.0f, 554.0f, light));
	list.push_back(std::make_shared<FlipNormal>(std::make_shared<RectXZ>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white)));
	list.push_back(std::make_shared<RectXZ>(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, white));
	list.push_back(std::make_shared<FlipNormal>(std::make_shared<RectXY>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white)));

	std::shared_ptr<Hitable> b1 = std::make_shared<Translate>(std::make_shared<RotateY>(std::make_shared<Box>(
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(165.0f, 165.0f, 165.0f), white), -18.0f), glm::vec3(130.0f, 0.0f, 65.0f));
	std::shared_ptr<Hitable> b2 = std::make_shared<Translate>(std::make_shared<RotateY>(std::make_shared<Box>(
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(165.0f, 330.0f, 165.0f), white), 15.0f), glm::vec3(265.0f, 0.0f, 295.0f));

	list.push_back(std::make_shared<ConstantMedium>(b1, 0.01f, std::make_shared<ConstantTexture>(glm::vec3(1.0f, 1.0f, 1.0f))));
	list.push_back(std::make_shared<ConstantMedium>(b2, 0.01f, std::make_shared<ConstantTexture>(glm::vec3(0.0f, 0.0f, 0.0f))));

	return std::make_unique<BVHNode>(list);
}

std::unique_ptr<Hitable> finalRandomSceneNextWeek()
{
	int nb = 20;

	std::vector<std::shared_ptr<Hitable> > list;
	std::vector<std::shared_ptr<Hitable> > boxlist;
	std::vector<std::shared_ptr<Hitable> > spherelist;

	auto white = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.73f, 0.73f, 0.73f)));
	auto ground = std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.48f, 0.83f, 0.53f)));

	for (int i = 0; i < nb; i++)
	{
		for (int j = 0; j < nb; j++)
		{
			float w = 100.0f;
			float x0 = -1000.0f + i * w;
			float z0 = -1000.0f + j * w;
			float y0 = 0.0f;
			float x1 = x0 + w;
			float y1 = 100.0f * (dis(gen) + 0.01f);
			float z1 = z0 + w;
			boxlist.push_back(std::make_shared<Box>(glm::vec3(x0, y0, z0), glm::vec3(x1, y1, z1), ground));
		}
	}

	list.push_back(std::make_shared<BVHNode>(boxlist));

	auto light = std::make_shared<DiffuseLight>(std::make_shared<ConstantTexture>(glm::vec3(7.0f, 7.0f, 7.0f)));
	list.push_back(std::make_shared<RectXZ>(123.f, 423.0f, 147.0f, 412.0f, 554.0f, light));

	glm::vec3 center(400.0f, 400.0f, 200.0f);
	list.push_back(std::make_shared<MovingSphere>(center, center + glm::vec3(30.0f, 0.0f, 0.0f), 0.0f, 1.0f, 50.0f, 
		std::make_shared<Lambertian>(std::make_shared<ConstantTexture>(glm::vec3(0.7f, 0.3f, 0.1f)))));
	list.push_back(std::make_shared<Sphere>(glm::vec3(260.0f,150.0f,45.0f), 50.0f, std::make_shared<Dielectric>(1.5f)));
	list.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 150.0f, 145.0f), 50.0f, std::make_shared<Metal>(glm::vec3(0.8f,0.8f,0.9f), 10.0f)));

	std::shared_ptr<Hitable> boundary = std::make_shared<Sphere>(glm::vec3(360.0f, 150.0f, 145.0f), 70.0f, std::make_shared<Dielectric>(1.5f));
	list.push_back(std::make_shared<ConstantMedium>(boundary, 0.2f, std::make_shared<ConstantTexture>(glm::vec3(0.2f, 0.4f, 0.9f))));
	boundary = std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, 0.0f), 5000.0f, std::make_shared<Dielectric>(1.5f));
	list.push_back(std::make_shared<ConstantMedium>(boundary, 0.0001f, std::make_shared<ConstantTexture>(glm::vec3(1.0f))));

	int widthTex, heightTex, chanelTex;
	unsigned char *tex_data = stbi_load("earthNight.jpg", &widthTex, &heightTex, &chanelTex, 0);
	auto imgTex = std::make_shared<ImageTexture>(tex_data, widthTex, heightTex);
	list.push_back(std::make_shared<Sphere>(glm::vec3(400.0f, 200.0f, 400.0f), 100.0f, std::make_shared<Lambertian>(imgTex)));

	auto perlinTex = std::make_shared<NoiseTexture>(0.1f);
	list.push_back(std::make_shared<Sphere>(glm::vec3(220.0f, 280.0f, 300.0f), 80.0f, std::make_shared<Lambertian>(perlinTex)));

	int ns = 1000;
	for (int j = 0; j < ns; j++)
	{
		spherelist.push_back(std::make_shared<Sphere>(glm::vec3(165.0f * dis(gen), 165.0f * dis(gen), 165.0f * dis(gen)), 10.0f, white));
	}
	list.push_back(std::make_shared<Translate>(std::make_shared<RotateY>(std::make_shared<BVHNode>(list), 15.0f), glm::vec3(-100.0f, 270.0f, 395.0f)));

	return std::make_unique<BVHNode>(list);
}

int main() {

	resetStat();

	// Set size
	int width = 200;
	int height = 100;
	int loopAA = 20;

	// TODO : find a better way to do this
	numberOfPrimaryRay = width * height * loopAA;

	PPM image(width, height);

	//std::unique_ptr<Hitable>  world = std::make_unique<BVHNode>(list);
	//std::unique_ptr<Hitable>  world(finalRandomScene());
	//std::unique_ptr<Hitable>  world(twoSphere());
	//std::unique_ptr<Hitable>  world(twoPerlinSphere());
	//std::unique_ptr<Hitable>  world(earth());
	//std::unique_ptr<Hitable>  world(simpleLight());
	//std::unique_ptr<Hitable>  world(cornellBox());
	std::unique_ptr<Hitable>  world(finalRandomSceneNextWeek());

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
				col += color(r, world, 0);
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