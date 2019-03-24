#pragma once

#include <array>


#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

// Reread this section from the tutorial, and maybe check scratchapixel article 
// https://www.scratchapixel.com/lessons/procedural-generation-virtual-worlds/procedural-patterns-noise-part-1
// https://www.scratchapixel.com/lessons/procedural-generation-virtual-worlds/perlin-noise-part-2

// TOCHECK There is probably mistakes

std::random_device rdPerlin;  //Will be used to obtain a seed for the random number engine
std::mt19937 genPerlin(rdPerlin()); //Standard mersenne_twister_engine seeded with rd()
std::uniform_real_distribution<> disPerlin(0.0f, 1.0f);

class Perlin
{

public :

	Perlin()
	{
		perlinGenerate(m_ranvec);
		perlinGeneratePerm(m_permX);
		perlinGeneratePerm(m_permY);
		perlinGeneratePerm(m_permZ);
	}

	float noise(const glm::vec3 &p) const
	{
		float u = p.x - floor(p.x);
		float v = p.y - floor(p.y);
		float w = p.z - floor(p.z);
		int i = floor(p.x);
		int j = floor(p.y);
		int k = floor(p.z);
		glm::vec3 c[2][2][2];
		for (int di = 0; di < 2; di++)
		{
			for (int dj = 0; dj < 2; dj++)
			{
				for (int dk = 0; dk < 2; dk++)
				{
					c[di][dj][dk] = m_ranvec[m_permX[(i + di) & 255] ^ m_permY[(j + dj) & 255] ^ m_permZ[(k + dk) & 255]];
				}
			}
		}

		return perlinInterpolation(c,u,v,w);
	}

	float turb(const glm::vec3& p, int depth = 7) const {
		float accum = 0;
		glm::vec3 temp_p = p;
		float weight = 1.0;
		for (int i = 0; i < depth; i++) {
			accum += weight*noise(temp_p);
			weight *= 0.5;
			temp_p *= 2;
		}
		return fabs(accum);
	}

private :

	void perlinGenerate(std::array<glm::vec3, 256> &p)
	{
		for (int i = 0; i < 256; i++)
		{
			p[i] = glm::normalize(glm::vec3(-1.0f + 2.0f * disPerlin(genPerlin), -1.0f + 2.0f * disPerlin(genPerlin), -1.0f + 2.0f * disPerlin(genPerlin)));
		}
	}

	void permute(std::array<int, 256> &p)
	{
		for (int i = p.size() -1; i > 0; i--)
		{
			int target = int(disPerlin(genPerlin) * (i +1));
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

	// TODO : Change this function et put it in a utility file
	float perlinInterpolation(glm::vec3 c[2][2][2], float u, float v, float w) const 
	{
		float uu = u*u*(3 - 2 * u);
		float vv = v*v*(3 - 2 * v);
		float ww = w*w*(3 - 2 * w);
		float accum = 0;
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				for (int k = 0; k < 2; k++) {
					glm::vec3 weight_v(u - i, v - j, w - k);
					accum += (i*uu + (1 - i)*(1 - uu))*
						(j*vv + (1 - j)*(1 - vv))*
						(k*ww + (1 - k)*(1 - ww))*dot(c[i][j][k], weight_v);
				}
		return accum;
	}


	// Check if the class vector is necessary
	std::array<glm::vec3, 256> m_ranvec;
	std::array<int, 256> m_permX;
	std::array<int, 256> m_permY;
	std::array<int, 256> m_permZ;

};

