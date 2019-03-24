#pragma once

#include "Material.h"


// TODO : Create a real light class

class DiffuseLight : public Material
{

public :

	DiffuseLight(std::shared_ptr<Texture> emit) : m_emit(emit) {}

	virtual bool scatter(const CastedRay &in, glm::vec3 &attenuation, CastedRay &scattered) const override
	{
		return false;
	}

	virtual glm::vec3 emitted(float u, float v, const glm::vec3& p) const override
	{
		return m_emit->getValue(u, v, p);
	}

private :
	
	std::shared_ptr<Texture> m_emit;

};