#pragma once

#include "HitableList.h"
#include "Rectangle.h"
#include "FlipNormal.h"

class Box : public Hitable
{

public :

	Box() {}
	Box(const glm::vec3 &min, const glm::vec3 &max, std::shared_ptr<Material> mat) : m_min(min), m_max(max), m_mat(mat)
	{
		std::vector<std::shared_ptr<Hitable> > list;

		list.push_back(std::make_shared<RectXY>(m_min.x, m_max.x, m_min.y, m_max.y, m_max.z, m_mat));
		list.push_back(std::make_shared<FlipNormal>(std::make_shared<RectXY>(m_min.x, m_max.x, m_min.y, m_max.y, m_min.z, m_mat)));

		list.push_back(std::make_shared<RectXZ>(m_min.x, m_max.x, m_min.z, m_max.z, m_max.y, m_mat));
		list.push_back(std::make_shared<FlipNormal>(std::make_shared<RectXZ>(m_min.x, m_max.x, m_min.z, m_max.z, m_min.y, m_mat)));

		list.push_back(std::make_shared<RectYZ>(m_min.y, m_max.y, m_min.z, m_max.z, m_max.x, m_mat));
		list.push_back(std::make_shared<FlipNormal>(std::make_shared<RectYZ>(m_min.y, m_max.y, m_min.z, m_max.z, m_min.x, m_mat)));

		m_sides = HitableList(list);

	}

	virtual bool intersect(CastedRay& r, float t_min, float t_max) const override
	{
		return m_sides.intersect(r, t_min, t_max);
	}

	virtual AABB getAABB() const override
	{
		return AABB(m_min, m_max);
	}

	virtual glm::vec3 center() const override
	{
		return (m_min + m_max) / 2.0f;
	}

private :

	glm::vec3 m_min, m_max;
	std::shared_ptr<Material> m_mat;
	HitableList m_sides;

};