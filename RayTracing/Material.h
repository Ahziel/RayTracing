#pragma once

#include "Hitable.h"
#include "Ray.h"
#include <random>

// TODO : Change for a real hemisphere sampler see :
// https://www.scratchapixel.com/lessons/3d-basic-rendering/global-illumination-path-tracing/global-illumination-path-tracing-practical-implementation
// http://www.pbr-book.org/3ed-2018/Monte_Carlo_Integration/2D_Sampling_with_Multidimensional_Transformations.html#UniformSampleHemisphere
//
// Maybe put it in another header file that I give to Material.h
Vec3f randomUnitSphere()
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(0.0, 1.0);

	Vec3f P;
	do {
		P = 2.0f * Vec3f(dis(gen), dis(gen), dis(gen)) - Vec3f(1.0f);
	} while (P.squareLength() >= 1.0f);
	return P;
}

// TODO : Understand it and like randomUnitSphere
// Maybe put it in another header file that I give to Material.h
Vec3f reflect(const Vec3f& v, const Vec3f& n)
{
	return v - 2.0f * dot(v, n) * n;
}

// TODO : Understand it (Snell Law)
// Maybe put it in another header file that I give to Material.h
bool refract(const Vec3f &v, const Vec3f &n, float niOverNt, Vec3f &refracted)
{
	Vec3f uv = normalize(v);
	float dt = dot(uv, n);
	float discriminant = 1.0f - niOverNt * niOverNt * (1.0 - dt * dt);
	if (discriminant > 0.0f)
	{
		refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
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
	virtual bool scatter(const Ray &in, HitRecord &rec, Vec3f &attenuation, Ray &scattered) const = 0;
};