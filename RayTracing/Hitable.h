#pragma once

#include "Ray.h"
#include <memory>

// Check why it's needed
class Material;

struct HitRecord
{
	// t value for the equation of the Ray
	float t;
	// Point that have been hit
	glm::vec3 P;
	// Normal of the hit point
	glm::vec3 N;
	// Material found
	// TODO : Change it for a smart pointer
	std::shared_ptr<Material> matPtr;
};

class Hitable
{
public : 
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
};