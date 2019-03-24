#pragma once

#include "Hitable.h"
#include "CastedRay.h"
#include "AABB.h"

class Translate : public Hitable
{

public :

	Translate(std::shared_ptr<Hitable> obj, const glm::vec3 offset) : m_obj(obj), m_offset(offset) {}

	virtual bool intersect(CastedRay& r, float t_min, float t_max) const override
	{
		CastedRay moved(r.origin() - m_offset, r.direction, r.time());
		if (m_obj->intersect(moved, t_min, t_max))
		{
			moved.m_rec.P += m_offset;
			r.setHitRec(moved.hitRec());
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual AABB getAABB() const override
	{
		AABB temp = m_obj->getAABB();
		temp.setMin(temp.getMin() + m_offset);
		temp.setMax(temp.getMax() + m_offset);
		return temp;
	}

	virtual glm::vec3 center() const override
	{
		return m_obj->center() + m_offset;
	}

private :

	std::shared_ptr<Hitable> m_obj;
	glm::vec3 m_offset;

};