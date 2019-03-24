#pragma once

#include "CastedRay.h"
#include "Hitable.h"
#include "AABB.h"

class FlipNormal : public Hitable
{

public : 

	FlipNormal() : m_ptr(nullptr) {}
	FlipNormal(std::shared_ptr<Hitable> ptr) : m_ptr(ptr) {}

	virtual bool intersect(CastedRay& r, float t_min, float t_max) const override
	{
		if (m_ptr->intersect(r, t_min, t_max))
		{
			r.m_rec.N = -r.hitRec().N;
			return true;
		}
		else
		{
			return false;
		}
	}

	virtual AABB getAABB() const override
	{
		return m_ptr->getAABB();
	}

	virtual glm::vec3 center() const override
	{
		return m_ptr->center();
	}

private : 

	std::shared_ptr<Hitable> m_ptr;

};
