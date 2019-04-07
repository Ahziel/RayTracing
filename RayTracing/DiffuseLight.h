#pragma once

#include "Material.h"


// TODO : Create a real light class

class DiffuseLight : public Material
{

public :

	DiffuseLight(std::shared_ptr<Texture> emit) : m_emit(emit) {}

	virtual bool scatter(const CastedRay &in, glm::vec3 &attenuation, CastedRay &scattered) const 
	{
		return false;
	}

	virtual glm::vec3 emitted(const CastedRay &in, float u, float v, const glm::vec3& p) const override
	{
		if (glm::dot(in.direction(), in.hitRec().N) < 0.0f)
		{
			return m_emit->getValue(u, v, p);
		}
		else
		{
			return glm::vec3(0.0f);
		}
	}

private :
	
	std::shared_ptr<Texture> m_emit;

};