#pragma once

#include <limits>
#include <iostream>

#include "Hitable.h"
#include "Isotropic.h"
#include "AABB.h"

class ConstantMedium : public Hitable
{

public :

	ConstantMedium(std::shared_ptr<Hitable> boundary, float density, std::shared_ptr<Texture> mat) : m_boundary(boundary), m_density(density)
	{
		m_mat = std::make_shared<Isotropic>(mat);
	}

	virtual bool intersect(CastedRay& r, float t_min, float t_max) const override
	{

		// TODO : Copy constructor for Casted Ray + Maybe find another way because castedray seems inconvenient
		CastedRay r1(r.origin(), r.direction(), r.time());
		CastedRay r2(r.origin(), r.direction(), r.time());

		if (m_boundary->intersect(r1, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max()))
		{
			if (m_boundary->intersect(r2, r1.hitRec().t + 0.0001f, std::numeric_limits<float>::max()))
			{
				// Need to check to better understand

				HitRecord rec1 = r1.hitRec();
				HitRecord rec2 = r2.hitRec();

				if (rec1.t < t_min)
				{
					rec1.t = t_min;
				}

				if (rec2.t > t_max)
				{
					rec2.t = t_max;
				}

				if (rec1.t >= rec2.t)
				{
					return false;
				}

				if (rec1.t < 0.0f)
				{
					rec1.t = 0.0f;
				}

				float distInsideBoundary = (rec2.t - rec1.t) * glm::length(r.direction());
				float hitDistance = -(1.0f / m_density) * log(disM(genM));

				if (hitDistance < distInsideBoundary)
				{
					r.m_rec.t = rec1.t + hitDistance / glm::length(r.direction());
					r.m_rec.P = r.pointAtParameter(r.m_rec.t);
					r.m_rec.N = glm::vec3(1.0f, 0.0f, 0.0f);  //arbitrary value
					r.m_rec.matPtr = m_mat;
					return true;
				}

			}
		}
		return false;
	}


	virtual AABB getAABB() const override
	{
		return m_boundary->getAABB();
	}


	virtual glm::vec3 center() const override
	{
		return m_boundary->center();
	}

private :

	std::shared_ptr<Hitable> m_boundary;
	float m_density;
	std::shared_ptr<Material> m_mat;

};