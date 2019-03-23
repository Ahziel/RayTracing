#pragma once

#include "Perlin.h"
#include "Texture.h"

class NoiseTexture : public Texture
{

public :
	
	NoiseTexture()  : m_noise(Perlin()), m_scale(1.0f) {}
	NoiseTexture(float scale) : m_noise(Perlin()), m_scale(scale) {}

	virtual glm::vec3 getValue(float u, float v, const glm::vec3 &p) const
	{
		// return  glm::vec3(1.0f) * 0.5f *(1 + m_noise.turb(m_scale * p));
		// return vec3(1.0f) * m_noise.turb(scale * p);
		return glm::vec3(1.0f) * 0.5f *(1.0f + sin(m_scale  *p.z + 5.f * m_noise.turb(m_scale * p)));
	}

private :

	Perlin m_noise;
	float m_scale;

};