#pragma once

#include "Material.h"

/*

TODO :  Recreate a new class Metal using Lamarche's project, PBRT and ScratchAPixel as support

*/

class Metal : public Material
{

public :
	Metal() : m_albedo(0.0f), m_fuzz(0.0f) {}
	Metal(const glm::vec3& albedo, const float& fuzz) : m_albedo(albedo)
	{
		if (fuzz < 1.0f)
		{
			m_fuzz = fuzz;
		}
		else
		{
			m_fuzz = 1.0f;
		}
	}
	~Metal() {}

	virtual bool scatter(const CastedRay &in, glm::vec3 &attenuation, CastedRay &scattered) const override
	{
		glm::vec3 reflected = reflect(normalize(in.direction()), in.hitRec().N);
		scattered = CastedRay(in.hitRec().P, reflected + m_fuzz * randomUnitSphere(), in.time());
		attenuation = m_albedo;
		return (glm::dot(scattered.direction(), in.hitRec().N) > 0.0f);
	}

private :
	glm::vec3 m_albedo;
	float m_fuzz;

};