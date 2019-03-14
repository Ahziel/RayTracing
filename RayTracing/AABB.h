#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

//  To get numeric limit
#include <limits>

class AABB
{
public :

	AABB()
	{
		float	inf = std::numeric_limits<float>::infinity();
		m_min = glm::vec3(-inf);
		m_max = glm::vec3(inf);
	}

	AABB(const glm::vec3 &min, const glm::vec3 &max) : m_min(min), m_max(max) {}


	// TODO : Check this function i'm not sure of it
	bool isEmpty()
	{
		return m_min.x > m_max.x || m_min.y > m_max.y || m_min.z > m_max.z;
	}

	// TODO : add inline ?
	// intersect function

private :

	glm::vec3 m_min;
	glm::vec3 m_max;

};