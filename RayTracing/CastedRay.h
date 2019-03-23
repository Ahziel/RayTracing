#pragma once

#include <memory>
#include <limits>

#include "Ray.h"
#include "Material.h"
#include "HitRecord.h"

struct HitRecord
{
	// t value for the equation of the Ray
	float t;
	// Point that have been hit
	glm::vec3 P;
	// Normal of the hit point
	glm::vec3 N;
	// uv coordinate of the hit point
	float u;
	float v;
	// Material found
	std::shared_ptr<Material> matPtr;

	// TODO : bool to check if the intersection is valid ?
};

class CastedRay : public Ray
{
public :

	CastedRay() : m_origin(0.0f), m_direction(0.0f), m_invDirection(0.0f), m_sign(0), m_time(0.0f)
	{
		m_rec.t = std::numeric_limits<float>::max();
		m_rec.P = glm::vec3(0.0f);
		m_rec.N = glm::vec3(0.0f);
		m_rec.matPtr = nullptr;
	}

	CastedRay(const glm::vec3& origin, const glm::vec3& direction, float time) : m_origin(origin), m_direction(glm::normalize(direction)), m_time(time)
	{
		m_invDirection = 1.0f / m_direction;
		m_sign.x = (m_direction.x < 0.0f);
		m_sign.y = (m_direction.y < 0.0f);
		m_sign.z = (m_direction.z < 0.0f);

		m_rec.t = std::numeric_limits<float>::max();
		m_rec.P = glm::vec3(0.0f);
		m_rec.N = glm::vec3(0.0f);
		m_rec.matPtr = nullptr;
	}
	~CastedRay() {}

	virtual glm::vec3 origin() const { return m_origin; }
	virtual glm::vec3 direction() const { return m_direction; }
	virtual glm::vec3 invDirection() const { return m_invDirection; }
	virtual glm::ivec3 sign() const { return m_sign; }
	virtual float time() const { return m_time; }
	HitRecord hitRec() const { return m_rec; }

	void setHitRec(HitRecord &rec)
	{
		m_rec = rec;
	}

	virtual glm::vec3 pointAtParameter(const float t) const { return m_origin + m_direction * t; }

	// Maybe put it back in private and create a class to handle HitRecord
	HitRecord m_rec;

private:

	glm::vec3 m_origin;
	glm::vec3 m_direction;
	glm::vec3 m_invDirection;
	glm::ivec3 m_sign;
	float m_time;
	
};
