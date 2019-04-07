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
	~Dielectric() {}

	virtual bool scatter(const CastedRay &in, ScatterRecord &srec) const
	{
		srec.isSpecular = true;
		srec.pdfPtr = 0;
		srec.attenuation = glm::vec3(1.0, 1.0, 1.0);
		glm::vec3 outwardNormal;
		glm::vec3 reflected = reflect(in.direction(), in.hitRec().N);
		float niOverNt;
		glm::vec3 refracted;
		float reflectProb;
		float cosine;
		if (glm::dot(in.direction(), in.hitRec().N) > 0.0f)
		{
			outwardNormal = -in.hitRec().N;
			niOverNt = m_indice;
			cosine = m_indice * glm::dot(in.direction(), in.hitRec().N) / glm::length(in.direction());
		}
		else
		{
			outwardNormal = in.hitRec().N;
			niOverNt = 1.0f / m_indice;
			cosine = -glm::dot(in.direction(), in.hitRec().N) / glm::length(in.direction());
		}

		if (refract(in.direction(), outwardNormal, niOverNt, refracted))
		{
			reflectProb = schlick(cosine, m_indice);
		}
		else
		{
			reflectProb = 1.0f;
		}

		if (disM(genM) < reflectProb)
		{
			srec.specularRay = CastedRay(in.hitRec().P, reflected, in.time());
		}
		else
		{
			srec.specularRay = CastedRay(in.hitRec().P, refracted, in.time());
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