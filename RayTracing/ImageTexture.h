#pragma once

#include <vector>

#include "Texture.h"

// TODO : Change this class to use vector of ivec3

class ImageTexture : public Texture
{

public : 

	ImageTexture() {}
	ImageTexture(unsigned char * pixels, int width, int height) : m_pixels(pixels), m_width(width), m_height(height) {}


	virtual glm::vec3 getValue(float u, float v, const glm::vec3 &p) const override
	{
		int i = (u)*m_width;
		int j = (1 - v)*m_height - 0.001;
		if (i < 0) i = 0;
		if (j < 0) j = 0;
		if (i > m_width - 1) i = m_width - 1;
		if (j > m_height - 1) j = m_height - 1;
		float r = int(m_pixels[3 * i + 3 * m_width*j]) / 255.0;
		float g = int(m_pixels[3 * i + 3 * m_width*j + 1]) / 255.0;
		float b = int(m_pixels[3 * i + 3 * m_width*j + 2]) / 255.0;
		return glm::vec3(r, g, b);
	}

private :
	unsigned char * m_pixels;
	int m_width;
	int m_height;

};