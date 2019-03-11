#pragma once

#include "Material.h"

/*

TODO :  Recreate a new class Dielectric using Lamarche's project, PBRT and ScratchAPixel as support

*/

class Dielectric : public Material
{

public:

	Dielectric() : m_indice(1.0f) {}
	Dielectric(float indice) : m_indice(indice) {}

	virtual bool scatter(const Ray &in, HitRecord &rec, Vec3f &attenuation, Ray &scattered) const override
	{

		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
		std::uniform_real_distribution<> dis(0.0f, 1.0f);


		Vec3f outwardNormal;
		Vec3f reflected = reflect(in.direction(), rec.N);
		float niOverNt;
		attenuation = Vec3f(1.0f, 1.0f, 1.0f);
		Vec3f refracted;
		float reflectProb;
		float cosine;
		if (dot(in.direction(), rec.N) > 0.0f)
		{
			outwardNormal = -rec.N;
			niOverNt = m_indice;
			cosine = m_indice * dot(in.direction(), rec.N) / in.direction().length();
		}
		else
		{
			outwardNormal = rec.N;
			niOverNt = 1.0f / m_indice;
			cosine = -dot(in.direction(), rec.N) / in.direction().length();
		}

		if (refract(in.direction(), outwardNormal, niOverNt, refracted))
		{
			reflectProb = schlick(cosine, m_indice);
		}
		else
		{
			scattered = Ray(rec.P, reflected);
			reflectProb = 1.0f;
		}

		if (dis(gen) < reflectProb)
		{
			scattered = Ray(rec.P, reflected);
		}
		else
		{
			scattered = Ray(rec.P, refracted);
		}
		return true;
	}

private:
	float m_indice;

	// TODO : Check how this function works and why it works
	float schlick(float cosine, float indice) const
	{
		float r0 = (1.0f - indice) / (1.0f + indice);
		r0 = r0 * r0;
		return r0 + (1.0f - r0) * std::pow((1.0f - cosine), 5);
	}

};