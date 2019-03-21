#pragma once

#include <memory>

#include "Texture.h"

// TODO : Clean this class ( pointer, ...)


class CheckerTexture : public Texture
{
	
public :

	CheckerTexture() : m_odd(nullptr), m_even(nullptr) {}
	CheckerTexture(std::shared_ptr<Texture> odd, std::shared_ptr<Texture> even) : m_odd(odd), m_even(even) {}
	~CheckerTexture() {}

	virtual glm::vec3 getValue(float u, float v, const glm::vec3 &p) const
	{
		float sines = sin(10.0 * p.x) * sin(10.0 * p.y) * sin(10.0 * p.z);

		if (sines < 0.0f)
		{
			return m_odd->getValue(u, v, p);
		}
		else
		{
			return m_even->getValue(u, v, p);
		}
	}

private :

	std::shared_ptr<Texture> m_odd;
	std::shared_ptr<Texture> m_even;

};
