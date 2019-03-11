#pragma once

#include "Material.h"

/*

TODO :  Recreate a new class Lambertian using Lamarche's project, PBRT and ScratchAPixel as support

*/


class Lambertian : public Material
{
public :

	// Constructors
	Lambertian() : m_albedo(0.0f) {}
	Lambertian(const Vec3f& albedo) : m_albedo(albedo) {}

	virtual bool scatter(const Ray &in, HitRecord &rec, Vec3f &attenuation, Ray &scattered) const override
	{
		Vec3f target = rec.P + rec.N + randomUnitSphere();
		scattered = Ray(rec.P, target - rec.P);
		attenuation = m_albedo;
		return true;
	}

private :
	Vec3f m_albedo;
};
