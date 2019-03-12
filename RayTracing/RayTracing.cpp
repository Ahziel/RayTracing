#include "stdafx.h"

#include "Ray.h"
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include "PPM.h"

#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"

#include <iostream>
#include <limits>
#include <vector>
#include <random>
#include <memory>

const float PI = 3.14159265359f;

std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<> dis(0.0f, 1.0f);

// TODO : Check to change this pointer for a smartPointer ?
glm::vec3 color(const Ray& r, std::unique_ptr<Hitable> &world, int depth)
{
	HitRecord rec;

	if (world->hit(r, 0.001f, std::numeric_limits<float>::max(), rec))
	{
		Ray scattered;
		glm::vec3 attenuation;

		if (depth < 50.0f && rec.matPtr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return glm::vec3(0.0f);
		}
	}
	else
	{
		glm::vec3 unit_direction = glm::normalize(r.direction());
		float t = 0.5f * (unit_direction.y + 1.0f);
		return  glm::vec3(1.0f) *(1.0f - t) + glm::vec3(0.5f, 0.7f, 1.0f) * t;
	}
}

std::unique_ptr<Hitable> finalRandomScene()
{
	std::vector<std::shared_ptr<Hitable> > list;
	list.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, -1000.0f, 0.0f), 1000, std::make_shared<Lambertian>(glm::vec3(0.5f, 0.5f, 0.5f))));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float chooseMat = float(dis(gen));
			glm::vec3 center(a + 0.9f * dis(gen), 0.2, b + 0.9f * dis(gen));
			if ((center - glm::vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f)
			{
				if (chooseMat < 0.8f) // Diffuse material
				{
					list.push_back(std::make_shared<Sphere>(center, 0.2f, std::make_shared<Lambertian>(glm::vec3(dis(gen) * dis(gen), dis(gen) * dis(gen), dis(gen) * dis(gen)))));
				}
				else if (chooseMat < 0.95f) // Metal material
				{
					list.push_back(std::make_shared<Sphere>(center, 0.2f, std::make_shared<Metal>(glm::vec3(0.5f * (1.0f + dis(gen)), 0.5f * (1.0f + dis(gen)), 0.5f * (1.0f + dis(gen))), 0.5f * dis(gen))));
				}
				else // Glass material
				{
					list.push_back(std::make_shared<Sphere>(center, 0.2f, std::make_shared<Dielectric>(1.5f)));
				}
			}
		}
	}

	list.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, std::make_shared<Dielectric>(1.5f)));
	list.push_back(std::make_shared<Sphere>(glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f, std::make_shared<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f))));
	list.push_back(std::make_shared<Sphere>(glm::vec3(4.0, 1.0f, 0.0f), 1.0f, std::make_shared<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.0f)));


	return std::make_unique<HitableList>(list);
}

int main() {

	// Set size
	int width = 600;
	int height = 300;
	int loopAA = 15;

	PPM image(width, height);

	std::vector<std::shared_ptr<Hitable>> list;
	list.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, std::make_shared<Lambertian>(glm::vec3(0.8f, 0.3f, 0.3f))));
	list.push_back(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, std::make_shared<Lambertian>(glm::vec3(0.8f, 0.8f, 0.0f))));
	list.push_back(std::make_shared<Sphere>(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 0.3f)));
	list.push_back(std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f, std::make_shared<Dielectric>(1.5f)));
	list.push_back(std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, -1.0f), -0.45f, std::make_shared<Dielectric>(1.5f)));

	//std::unique_ptr<Hitable>  world = std::make_unique<HitableList>(list);
	std::unique_ptr<Hitable>  world(finalRandomScene());

	// Camera information
	glm::vec3 origin(13.0f, 2.0f, 3.0f);
	glm::vec3 lookat(0.0f, 0.0f, 0.0f);
	float distToFocus = 10.0f;
	float aperture = 0.1f;

	Camera cam(origin, lookat, glm::vec3(0.0, 1.0, 0.0), 20.0f, float(width) / float(height), aperture, distToFocus);

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			glm::vec3 col(0.0f);
			for (int s = 0; s < loopAA; s++)
			{
				float u = float(i + dis(gen)) / float(width);
				float v = float(height - j + dis(gen)) / float(height);

				Ray r = cam.generateRay(u, v);
				col += color(r, world, 0);
			}

			col /= float(loopAA);
			col = glm::vec3(sqrt(col.x), sqrt(col.y), sqrt(col.z));

			int red = int(255.99 * col.x);
			int green = int(255.99 * col.y);
			int blue = int(255.99 * col.z);

			image[j][i] = glm::ivec3(red, green, blue);
		}
	}

	image.write("test");

}