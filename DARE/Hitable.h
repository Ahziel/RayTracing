#pragma once

#include "Ray.h"

// Check why it's needed
class Material;

struct HitRecord
{
	// t value for the equation of the Ray
	float t;
	// Point that have been hit
	Vec3f P;
	// Normal of the hit point
	Vec3f N;
	// Material found
	// TODO : Change it for a smart pointer
	Material *matPtr;
};

class Hitable
{
public : 
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
};