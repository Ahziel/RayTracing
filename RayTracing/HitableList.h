#pragma once

#include <vector>
#include <memory>

#include "Hitable.h"
#include "AABB.h"

class HitableList : public Hitable {

public:
	HitableList() {}
	// Here I have to use share_ptr because I copy the vector in the constructor
	// TODO : Check is it's the best way
	HitableList(std::vector< std::shared_ptr<Hitable>> l) { m_listHitables = l; }
	~HitableList(){}

	glm::vec3 center() const override
	{
		glm::vec3 center(0.0f);
		for (auto& s : m_listHitables) {
			center += s->center();
		}
		center /= m_listHitables.size();
		return center;
	}

	AABB getAABB() const override
	{
		AABB aabb;
		for (auto& s : m_listHitables) {
			aabb.update(s->getAABB());
		}
		return aabb;
	}

	virtual bool intersect(CastedRay &r, float t_min, float t_max) const override;
	// TODO : check if vector is the best
	std::vector<std::shared_ptr<Hitable>> m_listHitables;
};

bool HitableList::intersect(CastedRay &r, float t_min, float t_max) const {
	bool hitAnything = false;
	float closestSoFar = t_max;
	for (unsigned int i = 0; i < m_listHitables.size(); i++)
	{
		if (m_listHitables[i]->intersect(r, t_min, closestSoFar)) {
			hitAnything = true;
			closestSoFar = r.hitRec().t;
		}
	}
	return hitAnything;
}