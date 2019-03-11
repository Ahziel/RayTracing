#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>


class Ray
{
public :

	Ray() : m_origin(0), m_direction(0){}
	Ray(const glm::vec3& origin, const glm::vec3& direction) : m_origin(origin), m_direction(glm::normalize(direction)) {}
	glm::vec3 origin() const { return m_origin; }
	glm::vec3 direction() const { return m_direction; }
	glm::vec3 pointAtParameter(const float t) const { return m_origin + m_direction * t; }

private :

	glm::vec3 m_origin;
	glm::vec3 m_direction;

};