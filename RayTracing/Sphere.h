#pragma once

#include "Hitable.h"
#include "Stats.h"
#include "AABB.h"


#include <memory>

// TODO : Check if there is  a cycle with HitRecord

class Sphere : public Hitable
{

public :

	Sphere() : m_center(0.0f), m_radius(0.0f), m_mat(nullptr) {}
	Sphere(const glm::vec3& center, const float& radius, std::shared_ptr<Material> mat) : m_center(center), m_radius(radius), m_mat(mat) {}
	~Sphere() {}

	glm::vec3 center() const override { return m_center; }
	float radius() const { return m_radius; }
	// TODO : Check if reference is fine
	void setCenter(const glm::vec3& center) { m_center = center; }
	void setRadius(const float radius) { m_radius = radius; }

	AABB getAABB() const override
	{
		return AABB(m_center - glm::vec3(m_radius), m_center + glm::vec3(m_radius));
	}

	bool intersect(CastedRay& r, float t_min, float t_max) const override
	{
		// Add to global variable for stats
		numberOfRaySphereTest += 1;

		// oc is the distance between the origin of the ray and the center of the sphere
		glm::vec3 oc = r.origin() - m_center;

		float a = glm::dot(r.direction(), r.direction());
		float b = 2.0f * glm::dot(oc, r.direction());
		float c = glm::dot(oc, oc) - m_radius * m_radius;


		// Solve quadratic equation
		float discriminant = b*b - 4.0f * a*c;

		if (discriminant >= 0.0f)
		{
			float temp = (-b - sqrt(discriminant)) / (2.0f * a);
			if (temp < t_max && temp > t_min )
			{
				if (temp < r.hitRec().t)
				{
					HitRecord rec;
					rec.t = temp;
					rec.P = r.pointAtParameter(temp);
					rec.N = normalize(rec.P - m_center);
					// TODO : Change uv function
					getUV((rec.P - m_center) / m_radius, rec.u, rec.v);
					rec.matPtr = m_mat;
					r.setHitRec(rec);
				}

				// Add to global variable for stats
				numberOfRaySphereIntersection += 1;
				
				return true;
			}
			temp = (-b + sqrt(discriminant)) / (2.0f * a);
			if (temp < t_max && temp > t_min)
			{
				if (temp < r.hitRec().t)
				{
					HitRecord rec;
					rec.t = temp;
					rec.P = r.pointAtParameter(temp);
					rec.N = normalize(rec.P - m_center);
					// TODO : Change uv function
					getUV((rec.P - m_center)/m_radius, rec.u, rec.v);
					rec.matPtr = m_mat;
					r.setHitRec(rec);
				}
				
				// Add to global variable for stats
				numberOfRaySphereIntersection += 1;

				return true;
			}
		}
		return false;
	}

	// TODO : Change uv function
	void getUV(const glm::vec3 &point, float &u, float &v) const
	{
		float phi = atan2(point.z, point.x);
		float theta = asin(point.y);
		u = 1 - (phi + 3.14159265359f) / (2 * 3.14159265359f);
		v = (theta + 3.14159265359f / 2) / 3.14159265359f;
	}

private :
	glm::vec3 m_center;
	float m_radius;
	std::shared_ptr<Material> m_mat;
};