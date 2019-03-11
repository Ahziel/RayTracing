#pragma once

#include "Hitable.h"

class Sphere : public Hitable
{

public :

	Sphere() : m_center(0.0f), m_radius(0.0f) {}
	Sphere(const Vec3f& center, const float& radius, Material *mat) : m_center(center), m_radius(radius), m_mat(mat) {}

	Vec3f center() const { return m_center; }
	float radius() const { return m_radius; }
	// TODO : Check if reference is fine
	void setCenter(const Vec3f& center) { m_center = center; }
	void setRadius(const float radius) { m_radius = radius; }

	bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
	{
		// oc is the distance between the origin of the ray and the center of the sphere
		Vec3f oc = r.origin() - m_center;

		float a = dot(r.direction(), r.direction());
		float b = 2.0f * dot(oc, r.direction());
		float c = dot(oc, oc) - m_radius * m_radius;


		// Solve quadratic equation
		float discriminant = b*b - 4.0f * a*c;

		if (discriminant >= 0.0f)
		{
			float temp = (-b - sqrt(discriminant)) / (2.0f * a);
			if (temp < t_max && temp > t_min )
			{
				rec.t = temp;
				rec.P = r.pointAtParameter(temp);
				rec.N = normalize(rec.P - m_center);
				rec.matPtr = m_mat;
				return true;
			}
			temp = (-b + sqrt(discriminant)) / (2.0f * a);
			if (temp < t_max && temp > t_min)
			{
				rec.t = temp;
				rec.P = r.pointAtParameter(temp);
				rec.N = normalize(rec.P - m_center);
				rec.matPtr = m_mat;
				return true;
			}
		}
		return false;
	}

private :
	Vec3f m_center;
	float m_radius;
	Material *m_mat;
};