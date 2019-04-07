#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

class ONB
{

public :

	ONB () {}
	// TODO : Control if i > 0 && i < 2
	inline glm::vec3 operator[] (int i) const { return m_axis[i]; }

	glm::vec3 u() const { return m_axis[0];}
	glm::vec3 v() const { return m_axis[1]; }
	glm::vec3 w() const { return m_axis[2]; }

	glm::vec3 local(const float a, const float b, const float c) const
	{
		return a * m_axis[0] + b * m_axis[1] + c * m_axis[2];
	}

	glm::vec3 local(const glm::vec3 &a) const
	{
		return a.x * u() + a.y * v() + a.z * w();
	}

	void build(const glm::vec3 &n)
	{
		m_axis[2] = glm::normalize(n);

		glm::vec3 a;

		if (fabs(w().x) > 0.9f)
		{
			a = glm::vec3(0.0f, 1.0f, 0.0f);
		}
		else
		{
			a = glm::vec3(1.0f, 0.0f, 0.0f);
		}

		m_axis[1] = glm::normalize(glm::cross(w(), a));
		m_axis[0] = glm::cross(w(),v());

	}

private :

	glm::vec3 m_axis[3];

};