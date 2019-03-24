#pragma once

#include "Hitable.h"
#include "CastedRay.h"
#include "AABB.h"

// TODO : Create only one class to handle a quad with any type of rotation

class RectXY : public Hitable
{

public :

	RectXY() {}
	RectXY(float x0, float x1, float y0, float y1, float k, std::shared_ptr<Material> mat) : m_x0(x0), m_x1(x1), m_y0(y0), m_y1(y1), m_k(k), m_mat(mat) {}

	virtual bool intersect(CastedRay& r, float t_min, float t_max) const override
	{
		float t = (m_k - r.origin().z) / r.direction().z;
		if (t < t_min || t > t_max)
		{
			return false;
		}
		float x = r.origin().x + t*r.direction().x;
		float y = r.origin().y + t*r.direction().y;
		if (x < m_x0 || x > m_x1 || y < m_y0 || y > m_y1)
			return false;
		r.m_rec.u = (x - m_x0) / (m_x1 - m_x0);
		r.m_rec.v = (y - m_y0) / (m_y1 - m_y0);
		r.m_rec.t = t;
		r.m_rec.matPtr = m_mat;
		r.m_rec.P = r.pointAtParameter(t);
		r.m_rec.N = glm::vec3(0.0f, 0.0f, 1.0f);
		return true;
	}

	virtual AABB getAABB() const
	{
		return AABB(glm::vec3(m_x0, m_y0, m_k - 0.0001f), glm::vec3(m_x1, m_y1, m_k + 0.0001f));
	}

	virtual glm::vec3 center() const
	{
		return glm::vec3((m_x0 + m_x1) / 2.0f, (m_y0 + m_y1) / 2.0f, m_k);
	}

private :
	float m_x0, m_x1, m_y0, m_y1, m_k;
	std::shared_ptr<Material> m_mat;
};

class RectXZ : public Hitable
{

public:

	RectXZ() {}
	RectXZ(float x0, float x1, float z0, float z1, float k, std::shared_ptr<Material> mat) : m_x0(x0), m_x1(x1), m_z0(z0), m_z1(z1), m_k(k), m_mat(mat) {}

	virtual bool intersect(CastedRay& r, float t_min, float t_max) const override
	{
		float t = (m_k - r.origin().y) / r.direction().y;
		if (t < t_min || t > t_max)
		{
			return false;
		}
		float x = r.origin().x + t*r.direction().x;
		float z = r.origin().z + t*r.direction().z;
		if (x < m_x0 || x > m_x1 || z < m_z0 || z > m_z1)
			return false;
		r.m_rec.u = (x - m_x0) / (m_x1 - m_x0);
		r.m_rec.v = (z - m_z0) / (m_z1 - m_z0);
		r.m_rec.t = t;
		r.m_rec.matPtr = m_mat;
		r.m_rec.P = r.pointAtParameter(t);
		r.m_rec.N = glm::vec3(0.0f, 1.0f, 0.0f);
		return true;
	}

	virtual AABB getAABB() const
	{
		return AABB(glm::vec3(m_x0, m_k - 0.0001f, m_z0), glm::vec3(m_x1, m_k + 0.0001f, m_z1));
	}

	virtual glm::vec3 center() const
	{
		return glm::vec3((m_x0 + m_x1) / 2.0f, m_k, (m_z0 + m_z1) / 2.0f);
	}

private:
	float m_x0, m_x1, m_z0, m_z1, m_k;
	std::shared_ptr<Material> m_mat;
};

class RectYZ : public Hitable
{

public:

	RectYZ() {}
	RectYZ(float y0, float y1, float z0, float z1, float k, std::shared_ptr<Material> mat) : m_y0(y0), m_y1(y1), m_z0(z0), m_z1(z1), m_k(k), m_mat(mat) {}

	virtual bool intersect(CastedRay& r, float t_min, float t_max) const override
	{
		float t = (m_k - r.origin().x) / r.direction().x;
		if (t < t_min || t > t_max)
		{
			return false;
		}
		float y = r.origin().y + t*r.direction().y;
		float z = r.origin().z + t*r.direction().z;
		if (y < m_y0 || y > m_y1 || z < m_z0 || z > m_z1)
			return false;
		r.m_rec.u = (y - m_y0) / (m_y1 - m_y0);
		r.m_rec.v = (z - m_z0) / (m_z1 - m_z0);
		r.m_rec.t = t;
		r.m_rec.matPtr = m_mat;
		r.m_rec.P = r.pointAtParameter(t);
		r.m_rec.N = glm::vec3(1.0f, 0.0f, 0.0f);
		return true;
	}

	virtual AABB getAABB() const
	{
		return AABB(glm::vec3(m_k - 0.0001f, m_y0, m_z0), glm::vec3(m_k + 0.0001f, m_y1,  m_z1));
	}

	virtual glm::vec3 center() const
	{
		return glm::vec3(m_k, (m_y0 + m_y1) / 2.0f, (m_z0 + m_z1) / 2.0f);
	}

private:
	float m_y0, m_y1, m_z0, m_z1, m_k;
	std::shared_ptr<Material> m_mat;
};