#pragma once

#include <memory>

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

// Check why it's needed
class Material;
class AABB;
class CastedRay;

class Hitable
{
public : 
	virtual bool intersect(CastedRay& r, float t_min, float t_max) const = 0;
	virtual AABB getAABB() const = 0;
	virtual glm::vec3 center() const = 0;
	virtual float pdfValue(const glm::vec3 &o, const glm::vec3 &v) const { return 0.0f; }
	virtual glm::vec3 random(const glm::vec3 &o) const { return glm::vec3(1.0f, 0.0f, 0.0f); }
};