#pragma once

#include <vector>
#include "Hitable.h"

class HitableList : public Hitable {

public:
	HitableList() {}
	HitableList(std::vector<Hitable*> l) { m_listHitables = l; m_listSize = l.size(); }
	virtual bool hit(const Ray &r, float t_min, float t_max, HitRecord &rec) const;
	// TODO : check if vector is the best
	std::vector<Hitable*> m_listHitables;
	unsigned int m_listSize;
};

bool HitableList::hit(const Ray &r, float t_min, float t_max, HitRecord &rec) const {
	HitRecord tempRec;
	bool hitAnything = false;
	double closestSoFar = t_max;
	for (int i = 0; i < m_listSize; i++)
	{
		if (m_listHitables[i]->hit(r, t_min, closestSoFar, tempRec)) {
			hitAnything = true;
			closestSoFar = tempRec.t;
			rec = tempRec;
		}
	}
	return hitAnything;
}