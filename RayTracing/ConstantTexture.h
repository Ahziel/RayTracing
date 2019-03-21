#pragma once

#include "Texture.h"

class ConstantTexture : public Texture
{

public :

	ConstantTexture() {}
	ConstantTexture(const glm::vec3 color) : m_color(color) {}
	~ConstantTexture() {}

	virtual glm::vec3 getValue(float u, float v, const glm::vec3 &p) const
	{
		return m_color;
	}

private :
	glm::vec3 m_color;

};