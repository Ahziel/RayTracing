#pragma once

#include "Material.h"

/*

TODO :  Recreate a new class Metal using Lamarche's project, PBRT and ScratchAPixel as support

*/

class Metal : public Material
{

public :
	Metal() : m_albedo(0.0f), m_fuzz(0.0f) {}
	Metal(const Vec3f& albedo, const float& fuzz) : m_albedo(albedo)
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

	virtual bool scatter(const Ray &in, HitRecord &rec, Vec3f &attenuation, Ray &scattered) const override
	{
		Vec3f reflected = reflect(normalize(in.direction()),rec.N);
		scattered = Ray(rec.P, reflected + m_fuzz * randomUnitSphere());
		attenuation = m_albedo;
		return (dot(scattered.direction(), rec.N) > 0.0f);
	}

private :
	Vec3f m_albedo;
	float m_fuzz;

};