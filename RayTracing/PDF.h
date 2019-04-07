#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "ONB.h"
#include "Material.h"

class PDF
{

public :

	virtual float value(const glm::vec3 &direction) const = 0;
	virtual glm::vec3 generate() const = 0;

};

class CosinePDF : public PDF
{

public :

	CosinePDF(const glm::vec3 &w) { m_uvw.build(w); }

	virtual float value(const glm::vec3 &direction) const override
	{
		float cosine = glm::dot(glm::normalize(direction), m_uvw.w());
		if (cosine > 0.0f)
		{
			return cosine / 3.14159265f;
		}
		else
		{
			return 0.0f;
		}
	}

	virtual glm::vec3 generate() const override
	{
		return m_uvw.local(randomCosineDirection());
	}

private :
	ONB m_uvw;
};


class HitablePDF : public PDF
{

public:

	HitablePDF(std::shared_ptr<Hitable> &ptr, const glm::vec3 &origin) : m_ptr(ptr), m_origin(origin) { }

	virtual float value(const glm::vec3 &direction) const override
	{
		return m_ptr->pdfValue(m_origin, direction);
	}

	virtual glm::vec3 generate() const override
	{
		return m_ptr->random(m_origin);
	}

private:
	glm::vec3 m_origin;
	std::shared_ptr<Hitable> m_ptr;
};


class MixturePDF : public PDF
{

public:

	MixturePDF(std::shared_ptr<PDF> ptr1, std::shared_ptr<PDF> ptr2) : m_ptr1(ptr1), m_ptr2(ptr2) { }

	virtual float value(const glm::vec3 &direction) const override
	{
		return 0.5f * m_ptr1->value(direction) + 0.5f * m_ptr2->value(direction);
	}

	virtual glm::vec3 generate() const override
	{
		if (disM(genM) < 0.5f)
		{
			return m_ptr1->generate();
		}
		else
		{
			return m_ptr2->generate();
		}
	}

private:
	std::shared_ptr<PDF> m_ptr1;
	std::shared_ptr<PDF> m_ptr2;
};