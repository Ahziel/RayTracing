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

	bool intersect(const Ray& r, float t_min, float t_max, HitRecord& rec) const override
	{
		float entryT, exitT;
		HitRecord leftRec, rightRec;
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
				bool hitLeft = false;
				bool hitRight =false;

				bool interLeft = m_leftChild->getAABB().intersect(r, 0, std::numeric_limits<float>::max(), entryTL, exitTL);
				bool interRight = m_rightChild->getAABB().intersect(r, 0, std::numeric_limits<float>::max(), entryTR, exitTR);

				hitLeft = m_leftChild->intersect(r, t_min, t_max, leftRec);
				hitRight = m_rightChild->intersect(r, t_min, t_max, rightRec);

				/*if (entryTL < entryTR) {
					hitLeft = m_leftChild->intersect(r, t_min, t_max, leftRec);
					if (leftRec.t > entryTR)
					{
						hitRight = m_rightChild->intersect(r, t_min, t_max, rightRec);
					}
				}
				else {
					hitRight = m_rightChild->intersect(r, t_min, t_max, rightRec);
					if (rightRec.t > entryTL)
					{
						hitLeft = m_leftChild->intersect(r, t_min, t_max, leftRec);
					}
				}*/


				if (hitLeft && hitRight)
				{
					if (leftRec.t < rightRec.t)
					{
						rec = leftRec;
					}
					else
					{
						rec = rightRec;
					}
					hitAnything = true;
				}
				else if (hitLeft)
				{
					rec = leftRec;
					hitAnything = true;
				}
				else if (hitRight)
				{
					rec = rightRec;
					hitAnything = true;
				}

			}
			else
			{
				HitRecord tempRec;
				float closestSoFar = t_max;
				for (auto& s : m_hitables)
				{
					if (s->intersect(r, t_min, closestSoFar, tempRec)) {
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
	std::vector<std::shared_ptr<Hitable>> m_hitables;
	int m_sizeSubdiv;
	AABB m_box;
};