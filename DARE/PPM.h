#pragma once

#include <ostream>
#include <fstream>
#include "Vector.h"

/*

The class PPM create  a PPM image.
The image is written from left to right and from top to bottom.
i is for the position of the row and j is for the position of column

*/

class PPM {

public:

	PPM() {}

	PPM(unsigned int width, unsigned int height) : m_width(width), m_height(height), m_size(width * height)
	{
		m_pixels = new Vec3i[m_size];
	}

	~PPM() { delete[] m_pixels; }

	void set(const int i, const int j, const Vec3i color) { m_pixels[i * m_width + j] = color; }
	Vec3i get(const int i, const int j) const { return m_pixels[i * m_width + j]; }

	int width() const { return m_width; }
	int height() const { return m_height; }

	void write(const char* name)
	{
		std::ofstream ofs("test.ppm", std::ofstream::trunc); // Create file with overwrite if necessary

		ofs << "P3\n" << m_width << " " << m_height << "\n255\n";

		for (int j = 0; j < m_height; j++)
		{
			for (int i = 0; i < m_width; i++)
			{
				Vec3i pixel = m_pixels[j * m_width + i];

				ofs << pixel.x << " " << pixel.y << " " << pixel.z << "\n";
			}
		}

		ofs.close(); // then you close it and it's done
	}


	// Handle double bracket operator (TO DO : check if it's safe)
	const Vec3i* operator [] (int i) const
	{
		return &m_pixels[i * m_width];
	}

	Vec3i* operator [] (int i)
	{
		return &m_pixels[i * m_width];
	}

private:

	unsigned int m_width;
	unsigned int m_height;

	unsigned int m_size;

	// Array for RGB value
	Vec3i *m_pixels;
};


