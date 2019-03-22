#pragma once

#include "Perlin.h"
#include "Texture.h"

class NoiseTexture : public Texture
{

public :
	
	NoiseTexture()  : m_noise(Perlin()) {}

	virtual glm::vec3 getValue(float u, float v, const glm::vec3 &p) const
	{
		return glm::vec3(1.0f) * m_noise.noise(p);
	}

private :

	Perlin m_noise;

};