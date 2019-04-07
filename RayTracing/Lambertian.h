#pragma once

#include "Material.h"
#include "ONB.h"

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

	float scattering_pdf(const CastedRay &in, const CastedRay &scattered) const
	{
		float cosine = glm::dot(in.hitRec().N, glm::normalize(scattered.direction()));
		if (cosine < 0.0f)
		{
			cosine = 0.0f;
		}
		return (cosine / 3.14159265f);
	}

	virtual bool scatter(const CastedRay &in, ScatterRecord &srec) const
	{
		srec.isSpecular = false;
		srec.attenuation = m_albedo->getValue(in.hitRec().u, in.hitRec().v, in.hitRec().P);
		srec.pdfPtr = std::make_shared<CosinePDF>(in.hitRec().N);
		return true;
	}

private :
	std::shared_ptr<Texture> m_albedo;
};
