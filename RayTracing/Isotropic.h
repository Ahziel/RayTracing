#pragma once

#include "Material.h"

class Isotropic : public Material
{

public :

	Isotropic(std::shared_ptr<Texture> albedo) : m_albedo(albedo) {}

	virtual bool scatter(const CastedRay &in, glm::vec3 &attenuation, CastedRay &scattered) const override
	{
		scattered = CastedRay(in.hitRec().P, randomUnitSphere(), in.time());
		attenuation = m_albedo->getValue(in.hitRec().u, in.hitRec().v, in.hitRec().P);
		return true;
	}

private :

	std::shared_ptr<Texture> m_albedo;

};