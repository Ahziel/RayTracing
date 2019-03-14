#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>


class Ray
{
public :

	Ray() : m_origin(0), m_direction(0), m_time(0){}
	Ray(const glm::vec3& origin, const glm::vec3& direction, float time) : m_origin(origin), m_direction(glm::normalize(direction)), m_time(time) {}
	~Ray() {}

	glm::vec3 origin() const { return m_origin; }
	glm::vec3 direction() const { return m_direction; }
	float time() const { return m_time; }

	glm::vec3 pointAtParameter(const float t) const { return m_origin + m_direction * t; }

private :

	glm::vec3 m_origin;
	glm::vec3 m_direction;
	float m_time;

};