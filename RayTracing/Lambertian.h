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
	Lambertian(const glm::vec3& albedo) : m_albedo(albedo) {}
	~Lambertian() {}

	virtual bool scatter(const Ray &in, HitRecord &rec, glm::vec3 &attenuation, Ray &scattered) const override
	{
		glm::vec3 target = rec.P + rec.N + randomUnitSphere();
		scattered = Ray(rec.P, target - rec.P);
		attenuation = m_albedo;
		return true;
	}

private :
	glm::vec3 m_albedo;
};
