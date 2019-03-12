#pragma once

#include "Ray.h"
#include <random>
//Change this class to use matrix

glm::vec3 randomInUnitDisk()
{
	// Random - Need to replace
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(0.0f, 1.0f);

	glm::vec3 p;
	do {
		p = 2.0f * glm::vec3(dis(gen), dis(gen), 0.0f) - glm::vec3(1.0f, 1.0f, 0.0f);
	} while (glm::dot(p, p) >= 1.0f);
	return p;
}

class Camera {
public:
	Camera(glm::vec3 origin, glm::vec3 lookat, glm::vec3 vup, float vfov, float aspect, float aperture, float focusDist) {

		m_lensRadius = aperture / 2;

		// Put a global variable for PI
		float theta = vfov * 3.1415f / 180.0f;
		float halfHeight = tan(theta / 2);
		float halfWidth = aspect * halfHeight;

		m_origin = origin;

		m_w = normalize(origin - lookat);
		m_u = normalize(cross(vup, m_w));
		m_v = cross(m_w, m_u);

		m_lowerLeftCorner = m_origin - halfWidth * m_u * focusDist - halfHeight * m_v *focusDist - m_w * focusDist;
		m_horizontal = 2 * halfWidth * m_u * focusDist;
		m_vertical = 2 * halfHeight * m_v * focusDist;
	}
	~Camera() {}

	Ray generateRay(float u, float v) const
	{
		glm::vec3 rd = m_lensRadius * randomInUnitDisk();
		glm::vec3 offset = m_u * rd.x + m_v * rd.y;
		return Ray(m_origin + offset, m_lowerLeftCorner + u * m_horizontal + v * m_vertical - m_origin - offset);
	}

	// Basis
	glm::vec3 m_u, m_v, m_w;
	glm::vec3 m_origin;
	glm::vec3 m_lowerLeftCorner;
	glm::vec3 m_horizontal;
	glm::vec3 m_vertical;
	float m_lensRadius;
};