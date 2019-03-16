#pragma once

#include "Ray.h"
#include "Sphere.h"

#include <limits>
#include <algorithm>


class AABB
{
public :

	AABB()
	{
		float	inf = std::numeric_limits<float>::infinity();
		m_bounds[0] = glm::vec3(-inf);
		m_bounds[1] = glm::vec3(inf);
	}

	AABB(const glm::vec3 &min, const glm::vec3 &max)
	{
		m_bounds[0] = min;
		m_bounds[1] = max;
	}

	bool isEmpty()
	{
		bool result = false;
		for (int cpt = 0; cpt < 3; ++cpt)
		{
			result |= m_bounds[0][cpt] > m_bounds[1][cpt];
		}
		return result;
	}

	// SET

	void set(const Sphere &s)
	{
		m_bounds[0] = s.center() - glm::vec3(s.radius());
		m_bounds[1] = s.center() + glm::vec3(s.radius());
	}

	// UPDATE
	void update(const Sphere &s)
	{
		m_bounds[0] = getMin(m_bounds[0], s.center() - glm::vec3(s.radius()));
		m_bounds[1] = getMax(m_bounds[1], s.center() + glm::vec3(s.radius()));;
	}

	// TODO : add inline ?
	// Compare to Lamarche version and Peter Shirley version
	bool intersect(const Ray &r, float tmin, float tmax, float &entryT, float &exitT) const
	{
		float tmin, tmax, tymin, tymax, tzmin, tzmax;

		tmin = (m_bounds[r.sign().x].x - r.origin().x) * r.invDirection().x;
		tmax = (m_bounds[1 - r.sign().x].x - r.origin().x) * r.invDirection().x;
		tymin = (m_bounds[r.sign().y].y - r.origin().y) * r.invDirection().y;
		tymax = (m_bounds[1 - r.sign().y].y - r.origin().y) * r.invDirection().y;

		if ((tmin > tymax) || (tymin > tmax))
			return false;
		if (tymin > tmin)
			tmin = tymin;
		if (tymax < tmax)
			tmax = tymax;

		tzmin = (m_bounds[r.sign().z].z - r.origin().z) * r.invDirection().z;
		tzmax = (m_bounds[1 - r.sign().z].z - r.origin().z) * r.invDirection().z;

		if ((tmin > tzmax) || (tzmin > tmax))
			return false;
		if (tzmin > tmin)
			tmin = tzmin;
		if (tzmax < tmax)
			tmax = tzmax;

		entryT = tmin;
		exitT = tmax;
	}

private :

	glm::vec3 m_bounds[2];

	// UTILITIES

	// TODO : Find better name
	glm::vec3 getMin(glm::vec3 const & v1, glm::vec3 const & v2) const
	{
		glm::vec3 result;
		for (int cpt = 0; cpt < 3; ++cpt)
		{
			result[cpt] = ::std::min(v1[cpt], v2[cpt]);
		}
		return result;
	}

	// TODO : Find better name
	glm::vec3 getMax(glm::vec3 const & v1, glm::vec3 const & v2) const
	{
		glm::vec3 result;
		for (int cpt = 0; cpt < 3; ++cpt)
		{
			result[cpt] = ::std::max(v1[cpt], v2[cpt]);
		}
		return result;
	}

};