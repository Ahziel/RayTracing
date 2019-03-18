#pragma once

#include <random>
#include <vector>
#include <memory>
#include <iostream>

#include "Hitable.h"
#include "AABB.h"

class BVHNode : public Hitable
{
public:

	BVHNode() : m_leftChild(nullptr), m_rightChild(nullptr), m_sizeSubdiv(32) {}

	BVHNode(std::vector<std::shared_ptr<Hitable>> &hitables, int sizeSubdiv = 32) : m_sizeSubdiv(sizeSubdiv), m_leftChild(nullptr), m_rightChild(nullptr)
	{
		m_box = AABB();

		for (auto& s : hitables) {
			m_box.update(s->getAABB());
		}

		if (hitables.size() < m_sizeSubdiv)
		{
			m_hitables = hitables;
		}
		else
		{
			std::random_device rd;     // only used once to initialise (seed) engine
			std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
			std::uniform_int_distribution<int> uni(0, 2); // guaranteed unbiased

			int axis = uni(rng);

			// Sort on a axis unsing lambda function
			std::sort(hitables.begin(), hitables.end(), 
			[axis]( std::shared_ptr<Hitable> & a,  std::shared_ptr<Hitable> & b) -> bool
			{
				return a->center()[axis] > b->center()[axis];
			});

			std::vector<std::shared_ptr<Hitable>> leftList(hitables.begin(), hitables.begin() + hitables.size() / 2);
			std::vector<std::shared_ptr<Hitable>> rightList(hitables.begin() + hitables.size() / 2, hitables.end());


			m_leftChild = std::make_unique<BVHNode>(leftList);
			m_rightChild = std::make_unique<BVHNode>(rightList);
		}
	}

	~BVHNode(){}

	glm::vec3 center() const override
	{
		return (m_box.getMin() + m_box.getMax()) / 2.0f;
	}

	AABB getAABB() const override
	{
		return m_box;
	}

	bool intersect(CastedRay& r, float t_min, float t_max) const override
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

				hitAnything = m_leftChild->intersect(r, t_min, t_max) | m_rightChild->intersect(r, t_min, t_max);

				/*if (interLeft && interRight)
				{
					if (entryTL < entryTR) {
						hitAnything = m_leftChild->intersect(r, t_min, t_max);
						if (r.hitRec().t > entryTR)
						{
							hitAnything = hitAnything || m_rightChild->intersect(r, t_min, t_max);
						}
					}
					else {
						hitAnything = m_rightChild->intersect(r, t_min, t_max);
						if (r.hitRec().t > entryTL)
						{
						hitAnything = hitAnything || m_leftChild->intersect(r, t_min, t_max);
						}
					}
				}
				else if (interLeft)
				{
				hitAnything = m_leftChild->intersect(r, t_min, t_max);
				}
				else if (interRight)
				{
				hitAnything = m_rightChild->intersect(r, t_min, t_max);
				}*/

			}
			else
			{
				float closestSoFar = t_max;

				for (auto& s : m_hitables)
				{
					if (s->intersect(r, t_min, closestSoFar)) {
						hitAnything = true;
						closestSoFar = r.hitRec().t;
					}
				}
			}
			return hitAnything;
		}
	}

private :

	std::unique_ptr<BVHNode> m_leftChild;
	std::unique_ptr<BVHNode> m_rightChild;
	std::vector<std::shared_ptr<Hitable>> m_hitables;
	int m_sizeSubdiv;
	AABB m_box;
};