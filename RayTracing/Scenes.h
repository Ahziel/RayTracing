#pragma once

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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <memory>

std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<> dis(0.0f, 1.0f);

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
	auto aluminium = std::make_shared<Metal>(glm::vec3(0.8f, 0.85f, 0.88f), 0.0f);
	auto glass = std::make_shared<Dielectric>(1.5f);
	

	std::vector<std::shared_ptr<Hitable> > list;

	list.push_back(std::make_shared<FlipNormal>(std::make_shared<RectYZ>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, green)));
	list.push_back(std::make_shared<RectYZ>(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, red));
	list.push_back(std::make_shared<FlipNormal>(std::make_shared<RectXZ>(213.0f, 343.0f, 227.0f, 332.0f, 554.0f, light)));
	list.push_back(std::make_shared<FlipNormal>(std::make_shared<RectXZ>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white)));
	list.push_back(std::make_shared<RectXZ>(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, white));
	list.push_back(std::make_shared<FlipNormal>(std::make_shared<RectXY>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white)));

	/*std::shared_ptr<Hitable> b1 = std::make_shared<Translate>(std::make_shared<RotateY>(std::make_shared<Box>(
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(165.0f, 165.0f, 165.0f), white), -18.0f), glm::vec3(130.0f, 0.0f, 65.0f));*/
	list.push_back(std::make_shared<Sphere>(glm::vec3(190, 90, 190), 90, glass));

	std::shared_ptr<Hitable> b2 = std::make_shared<Translate>(std::make_shared<RotateY>(std::make_shared<Box>(
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(165.0f, 330.0f, 165.0f), white), 15.0f), glm::vec3(265.0f, 0.0f, 295.0f));

	//list.push_back(b1);
	list.push_back(b2);

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
	list.push_back(std::make_shared<Sphere>(glm::vec3(260.0f, 150.0f, 45.0f), 50.0f, std::make_shared<Dielectric>(1.5f)));
	list.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 150.0f, 145.0f), 50.0f, std::make_shared<Metal>(glm::vec3(0.8f, 0.8f, 0.9f), 10.0f)));

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