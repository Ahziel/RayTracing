#pragma once

#include <array>


#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "Random.h"


// Reread this section from the tutorial, and maybe check scratchapixel article 
// https://www.scratchapixel.com/lessons/procedural-generation-virtual-worlds/procedural-patterns-noise-part-1
// https://www.scratchapixel.com/lessons/procedural-generation-virtual-worlds/perlin-noise-part-2

class Perlin
{

public :

	Perlin()
	{
		perlinGenerate(m_ranfloat);
		perlinGeneratePerm(m_permX);
		perlinGeneratePerm(m_permY);
		perlinGeneratePerm(m_permZ);
	}

	float noise(const glm::vec3 &p) const
	{
		float u = p.x - floor(p.x);
		float v = p.y - floor(p.y);
		float w = p.z - floor(p.z);

		int i = int(4.0f * p.x) & 255;
		int j = int(4.0f * p.y) & 255;
		int k = int(4.0f * p.z) & 255;

		return m_ranfloat[m_permX[i] ^ m_permY[j] ^ m_permZ[k]];
	}

private :

	void perlinGenerate(std::array<float, 256> &p)
	{
		for (auto & element : p) {
			element = float(dis(gen));
		}
	}

	void permute(std::array<int, 256> &p)
	{
		for (int i = p.size() -1; i > 0; i--)
		{
			int target = int(dis(gen) * (i +1));
			int tmp = p[i];
			p[i] = p[target];
			p[target] = tmp;
		}
	}

	void perlinGeneratePerm(std::array<int, 256> &p)
	{
		for (int i = 0; i < 256; i++)
		{
			p[i] = i;
		}
		permute(p);
	}

	// Check if the class vector is necessary
	std::array<float, 256> m_ranfloat;
	std::array<int, 256> m_permX;
	std::array<int, 256> m_permY;
	std::array<int, 256> m_permZ;

};

