#pragma once

#include "Hitable.h"
#include "CastedRay.h"
#include "Texture.h"

#include <random>

std::random_device rdM;  //Will be used to obtain a seed for the random number engine
std::mt19937 genM(rdM()); //Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<> disM(0.0f, 1.0f);

// TODO : Change for a real hemisphere sampler see :
// https://www.scratchapixel.com/lessons/3d-basic-rendering/global-illumination-path-tracing/global-illumination-path-tracing-practical-implementation
// http://www.pbr-book.org/3ed-2018/Monte_Carlo_Integration/2D_Sampling_with_Multidimensional_Transformations.html#UniformSampleHemisphere
//
// Maybe put it in another header file that I give to Material.h
glm::vec3 randomUnitSphere()
{

	glm::vec3 P;
	do {
		P = 2.0f * glm::vec3(disM(genM), disM(genM), disM(genM)) - glm::vec3(1.0f);
	} while (glm::dot(P,P) >= 1.0f); // TODO : Check for P.length() * P.length(), before i used my gunction squaredLength
	return P;
}

// TODO : Understand it and like randomUnitSphere
// Maybe put it in another header file that I give to Material.h
glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n)
{
	return v - 2.0f * glm::dot(v, n) * n;
}

// TODO : Understand it (Snell Law)
// Maybe put it in another header file that I give to Material.h
bool refract(const glm::vec3 &v, const glm::vec3 &n, float niOverNt, glm::vec3 &refracted)
{
	glm::vec3 uv = normalize(v);
	float dt = glm::dot(uv, n);
	float discriminant = 1.0f - (niOverNt * niOverNt * (1.0f - dt * dt));
	if (discriminant > 0.0f)
	{
		refracted = niOverNt * (uv - n * dt) - (n * sqrt(discriminant));
		return true;
	}
	else
	{
		return false;
	}
}

class Material
{
public:
	virtual bool scatter(const CastedRay &in, glm::vec3 &attenuation, CastedRay &scattered) const = 0;
	virtual glm::vec3 emitted(float u, float v, const glm::vec3& p) const { return glm::vec3(0.0f, 0.0f, 0.0f);}
};