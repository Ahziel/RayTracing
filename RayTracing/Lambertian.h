#pragma once

#include "Material.h"

/*

TODO :  Recreate a new class Lambertian using Lamarche's project, PBRT and ScratchAPixel as support

*/


class Lambertian : public Material
{
public :

	// Constructors
	Lambertian() : m_albedo(nullptr) {}
	Lambertian(std::shared_ptr<Texture> albedo) : m_albedo(albedo) {}
	~Lambertian() {}

	virtual bool scatter(const CastedRay &in, glm::vec3 &attenuation, CastedRay &scattered) const override
	{
		glm::vec3 target = in.hitRec().P + in.hitRec().N + randomUnitSphere();
		scattered = CastedRay(in.hitRec().P, target - in.hitRec().P, in.time());
		attenuation = m_albedo->getValue(0, 0, in.hitRec().P);
		return true;
	}

private :
	std::shared_ptr<Texture> m_albedo;
};
