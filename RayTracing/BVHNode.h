#pragma once

#include <random>
#include <vector>
#include <memory>

#include "Hitable.h"
#include "AABB.h"

class BVHNode : public Hitable
{
public:

	BVHNode() : m_leftChild(nullptr), m_rightChild(nullptr), m_hitables(), m_sizeSubdiv(32), m_box() {}

	BVHNode(const std::vector<Hitable> &hitables, int sizeSubdiv = 32) : m_sizeSubdiv(sizeSubdiv), m_leftChild(nullptr), m_rightChild(nullptr)
	{
		m_box = AABB();

		for (auto& s : hitables) {
			m_box.update(s.getAABB());
		}

		if (!hitables.size() > m_sizeSubdiv)
		{
			m_hitables = hitables;
		}
		else
		{
			std::vector<Hitable> leftList;
			std::vector<Hitable> rightList;

			std::random_device rd;     // only used once to initialise (seed) engine
			std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
			std::uniform_int_distribution<int> uni(0, 2); // guaranteed unbiased

			int axis = uni(rng);

			// Sort on a axis unsing lambda function
			std::sort(hitables.begin(), hitables.end(), 
			[axis](const Hitable & a, const Hitable & b) -> bool
			{
				return a.center()[axis] > b.center()[axis];
			});

			std::vector<Hitable> leftList(hitables.begin(), hitables.begin() + hitables.size() / 2);
			std::vector<Hitable> rightList(hitables.begin() + hitables.size() / 2, hitables.end());


			m_leftChild = std::make_unique<BVHNode>(leftList);
			m_rightChild = std::make_unique<BVHNode>(rightList);
		}
	}

	~BVHNode(){}

	void init(const std::vector<Hitable> &hitables, int sizeSubdiv = 32)
	{
		m_box = AABB();

		for (auto& s : hitables) {
			m_box.update(s.getAABB());
		}

		if (!hitables.size() > m_sizeSubdiv)
		{
			m_hitables = hitables;
		}
		else
		{
			std::vector<Hitable> leftList;
			std::vector<Hitable> rightList;

			std::random_device rd;     // only used once to initialise (seed) engine
			std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
			std::uniform_int_distribution<int> uni(0, 2); // guaranteed unbiased

			int axis = uni(rng);

			// Sort on a axis unsing lambda function
			sort(hitables.begin(), hitables.end(),
				[axis](const Hitable & a, const Hitable & b) -> bool
			{
				return a.center()[axis] > b.center()[axis];
			});

			std::vector<Hitable> leftList(hitables.begin(), hitables.begin() + hitables.size() / 2);
			std::vector<Hitable> rightList(hitables.begin() + hitables.size() / 2, hitables.end());


			m_leftChild = std::make_unique<BVHNode>(leftList);
			m_rightChild = std::make_unique<BVHNode>(rightList);
		}
	}

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