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

	virtual bool  scatter(const CastedRay &in, ScatterRecord &srec) const
	{
		glm::vec3 reflected = reflect(in.direction(), in.hitRec().N);
		srec.specularRay = CastedRay(in.hitRec().P, reflected + m_fuzz * randomUnitSphere(), in.time());
		srec.attenuation = m_albedo;
		srec.isSpecular = true;
		srec.pdfPtr = nullptr;
		return true;
	}

private :
	glm::vec3 m_albedo;
	float m_fuzz;

};