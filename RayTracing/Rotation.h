#pragma once

#include "Hitable.h"
#include "AABB.h"

class RotateY : public Hitable
{

public :

	RotateY(std::shared_ptr<Hitable> obj, float angle) : m_obj(obj)
	{
		float radians = (3.14159265359f / 180.0f) * angle;
		m_sinTheta = sin(radians);
		m_cosTheta = cos(radians);
		glm::vec3 min(-std::numeric_limits<float>::max());
		glm::vec3 max(std::numeric_limits<float>::max());
		m_aabb = obj->getAABB();

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					float x = i * m_aabb.getMax().x + (1 - i) * m_aabb.getMin().x;
					float y = j * m_aabb.getMax().y + (1 - j) * m_aabb.getMin().y;
					float z = k * m_aabb.getMax().z + (1 - k) * m_aabb.getMin().z;

					float newx = m_cosTheta * x + m_sinTheta * z;
					float newz = -m_sinTheta * x + m_cosTheta * z;

					glm::vec3 tester(newx, y, newz);

					for (int c = 0; c < 3; c++)
					{
						if (tester[c] > max[c])
						{
							max[c] = tester[c];
						}
						if (tester[c] < max[c])
						{
							min[c] = tester[c];
						}
					}
				}
			}
		}
		m_aabb = AABB(min, max);
	}

	virtual bool intersect(CastedRay& r, float t_min, float t_max) const override
	{
		glm::vec3 origin = r.origin();
		glm::vec3 direction = r.direction();

		origin.x = m_cosTheta * r.origin().x - m_sinTheta * r.origin().z;
		origin.z = m_sinTheta * r.origin().x + m_cosTheta * r.origin().z;

		direction.x = m_cosTheta * r.direction().x - m_sinTheta * r.direction().z;
		direction.z = m_sinTheta * r.direction().x + m_cosTheta * r.direction().z;

		CastedRay rotated(origin, direction, r.time());

		if (m_obj->intersect(rotated, t_min, t_max))
		{
			glm::vec3 P = rotated.hitRec().P;
			glm::vec3 N = rotated.hitRec().N;

			P.x = m_cosTheta * rotated.hitRec().P.x + m_sinTheta * rotated.hitRec().P.z;
			P.z = -m_sinTheta * rotated.hitRec().P.x + m_cosTheta * rotated.hitRec().P.z;

			N.x = m_cosTheta * rotated.hitRec().N.x + m_sinTheta * rotated.hitRec().N.z;
			N.z = -m_sinTheta * rotated.hitRec().N.x + m_cosTheta * rotated.hitRec().N.z;

			r.setHitRec(rotated.hitRec());

			r.m_rec.P = P;
			r.m_rec.N = N;

			return true;
		}
		else
		{
			return false;
		}
	}

	virtual AABB getAABB() const override
	{
		return m_aabb;
	}

	virtual glm::vec3 center() const override
	{
		return (m_aabb.getMax() + m_aabb.getMin()) / 2.0f;
	}

private :

	std::shared_ptr<Hitable> m_obj;
	float m_sinTheta, m_cosTheta;
	bool m_hasBox;
	AABB m_aabb;

};