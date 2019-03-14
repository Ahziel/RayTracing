#pragma once

#include <vector>
#include <memory>
#include "Hitable.h"

class HitableList : public Hitable {

public:
	HitableList() {}
	// Here I have to use share_ptr because I copy the vector in the constructor
	// TODO : Check is it's the best way
	HitableList(std::vector< std::shared_ptr<Hitable>> l) { m_listHitables = l; m_listSize = l.size(); }
	~HitableList(){}


	virtual bool intersect(const Ray &r, float t_min, float t_max, HitRecord &rec) const;
	// TODO : check if vector is the best
	std::vector<std::shared_ptr<Hitable>> m_listHitables;
	unsigned int m_listSize;
};

bool HitableList::intersect(const Ray &r, float t_min, float t_max, HitRecord &rec) const {
	HitRecord tempRec;
	bool hitAnything = false;
	float closestSoFar = t_max;
	for (unsigned int i = 0; i < m_listSize; i++)
	{
		if (m_listHitables[i]->intersect(r, t_min, closestSoFar, tempRec)) {
			hitAnything = true;
			closestSoFar = tempRec.t;
			rec = tempRec;
		}
	}
	return hitAnything;
}