#pragma once

#include "Vector.h"


class Ray
{
public :

	Ray() : m_origin(0), m_direction(0){}
	Ray(const Vec3f& origin, const Vec3f& direction) : m_origin(origin), m_direction(direction) {}
	Vec3f origin() const { return m_origin; }
	Vec3f direction() const { return m_direction; }
	Vec3f pointAtParameter(const float t) const { return m_origin + m_direction * t; }

private :

	Vec3f m_origin;
	Vec3f m_direction;

};