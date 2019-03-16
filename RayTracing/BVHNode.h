#pragma once

#include <random>
#include <vector>
#include <memory>

#include "Hitable.h"
#include "AABB.h"

class BVHNode : Hitable
{
public:

	BVHNode() {}

	BVHNode(const std::vector<Hitable> &hitables, int sizeSubdiv = 32) : m_hitables(hitables), m_sizeSubdiv(sizeSubdiv), m_leftChild(nullptr), m_rightChild(nullptr)
	{
	}

	~BVHNode(){}

	void init(const std::vector<Hitable> &spheres, int sizeSubdiv = 32)
	{
	}

	AABB getAABB() const
	{
		return m_box;
	}

	bool intersect(const Ray& r, float t_min, float t_max, HitRecord& rec) const
	{
		float entryT, exitT;
		bool hitAnything = false;
		if (!m_box.intersect(r, t_min, t_max, entryT, exitT))
		{
			return hitAnything;
		}
		else
		{
			if (m_hitables.size() == 0)
			{
				float entryTL, exitTL;
				float entryTR, exitTR;
				bool interLeft = m_leftChild->getAABB().intersect(r, 0, std::numeric_limits<float>::max(), entryTL, exitTL);
				bool interRight = m_rightChild->getAABB().intersect(r, 0, std::numeric_limits<float>::max(), entryTR, exitTR);

				if (entryTL < entryTR) {
					hitAnything = m_leftChild->intersect(r, t_min, t_max, rec);
					if ( rec.t > entryTR)
					{
						hitAnything = hitAnything | m_rightChild->intersect(r, t_min, t_max, rec);
					}
				}
				else {
					hitAnything = m_rightChild->intersect(r, t_min, t_max, rec);
					if (rec.t > entryTL)
					{
						hitAnything = hitAnything | m_leftChild->intersect(r, t_min, t_max, rec);
					}
				}
			}
			else
			{
				HitRecord tempRec;
				float closestSoFar = t_max;
				for (auto& s : m_hitables)
				{
					if (s.intersect(r, t_min, closestSoFar, tempRec)) {
						hitAnything = true;
						closestSoFar = tempRec.t;
						rec = tempRec;
					}
				}
			}
			return hitAnything;
		}
	}

private :

	std::unique_ptr<BVHNode> m_leftChild;
	std::unique_ptr<BVHNode> m_rightChild;
	// TODO : Replace with HitableList ?
	std::vector<Hitable> m_hitables;
	int m_sizeSubdiv;
	AABB m_box;
};