#pragma once

#include "Hitable.h"
#include "CastedRay.h"

#include <limits>
#include <algorithm>


class AABB
{
public :

	AABB()
	{
		float	inf = std::numeric_limits<float>::infinity();
		m_bounds[0] = glm::vec3(inf);
		m_bounds[1] = glm::vec3(-inf);
	}

	AABB(const glm::vec3 &min, const glm::vec3 &max)
	{
		m_bounds[0] = min;
		m_bounds[1] = max;
	}

	glm::vec3 getMin() const
	{
		return m_bounds[0];
	}

	glm::vec3 getMax() const
	{
		return m_bounds[1];
	}

	bool isEmpty() const
	{
		bool result = false;
		for (int cpt = 0; cpt < 3; ++cpt)
		{
			result |= m_bounds[0][cpt] > m_bounds[1][cpt];
		}
		return result;
	}

	// UPDATE
	void update(const AABB &aabb)
	{
		m_bounds[0] = createMin(m_bounds[0], aabb.getMin());
		m_bounds[1] = createMax(m_bounds[1], aabb.getMax());
	}

	// TODO : add inline ?
	// Compare to Lamarche version and Peter Shirley version
	bool intersect(const Ray &r, float tmin, float tmax, float &entryT, float &exitT) const
	{
		float tymin, tymax, tzmin, tzmax;

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
		return true;
	}

	bool intersect(const CastedRay &r, float tmin, float tmax, float &entryT, float &exitT) const
	{
		float tymin, tymax, tzmin, tzmax;

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
		return true;
	}
private :

	glm::vec3 m_bounds[2];

	// UTILITIES

	// TODO : Find better name
	glm::vec3 createMin(glm::vec3 const & v1, glm::vec3 const & v2) const
	{
		glm::vec3 result;
		for (int cpt = 0; cpt < 3; ++cpt)
		{
			result[cpt] = ::std::min(v1[cpt], v2[cpt]);
		}
		return result;
	}

	// TODO : Find better name
	glm::vec3 createMax(glm::vec3 const & v1, glm::vec3 const & v2) const
	{
		glm::vec3 result;
		for (int cpt = 0; cpt < 3; ++cpt)
		{
			result[cpt] = ::std::max(v1[cpt], v2[cpt]);
		}
		return result;
	}

};