#pragma once

#include "Hitable.h"

class Translate : public Hitable
{

public :

	Translate(std::shared_ptr<Hitable> obj, const glm::vec3 offset) : m_obj(obj), m_offset(offset) {}

private :

	std::shared_ptr<Hitable> m_obj;
	glm::vec3 m_offset;

};