#pragma once

#include "Hitable.h"

#include "Stats.h"

// TODO : Find a proper way to handle moving objects

class MovingSphere : public Hitable
{

public :

	MovingSphere() : m_center0(0.0f), m_center1(0.0f), m_time0(0.0f), m_time1(0.0f), m_radius(0.0f), m_mat(nullptr) {}
	MovingSphere(const glm::vec3& center0, const glm::vec3& center1, const float& time0, const float& time1, const float& radius, std::shared_ptr<Material> mat)
		: m_center0(center0), m_center1(center1), m_time0(time0), m_time1(time1), m_radius(radius), m_mat(mat) {}
	~MovingSphere() {}

	virtual bool intersect(const Ray& r, float t_min, float t_max, HitRecord& rec) const override
	{
		// Add to global variable for stats
		numberOfRaySphereTest += 1;

		// oc is the distance between the origin of the ray and the center of the sphere
		glm::vec3 oc = r.origin() - center(r.time());

		float a = glm::dot(r.direction(), r.direction());
		float b = 2.0f * glm::dot(oc, r.direction());
		float c = glm::dot(oc, oc) - m_radius * m_radius;


		// Solve quadratic equation
		float discriminant = b*b - 4.0f * a*c;

		if (discriminant >= 0.0f)
		{
			float temp = (-b - sqrt(discriminant)) / (2.0f * a);
			if (temp < t_max && temp > t_min)
			{
				rec.t = temp;
				rec.P = r.pointAtParameter(temp);
				rec.N = normalize(rec.P - center(r.time()));
				rec.matPtr = m_mat;

				// Add to global variable for stats
				numberOfRaySphereIntersection += 1;

				return true;
			}
			temp = (-b + sqrt(discriminant)) / (2.0f * a);
			if (temp < t_max && temp > t_min)
			{
				rec.t = temp;
				rec.P = r.pointAtParameter(temp);
				rec.N = normalize(rec.P - center(r.time()));
				rec.matPtr = m_mat;

				// Add to global variable for stats
				numberOfRaySphereIntersection += 1;

				return true;
			}
		}
		return false;
	}

	glm::vec3 center(float time) const
	{
		return m_center0 + ((time - m_time0) / (m_time1 - m_time0)) * (m_center1 - m_center0);
	}

private :

	glm::vec3 m_center0, m_center1;
	float m_time0, m_time1;
	float m_radius;
	std::shared_ptr<Material> m_mat;

};