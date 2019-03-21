#pragma once

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

class Texture
{
	
public :
	virtual glm::vec3 getValue(float u, float v, const glm::vec3 &p) const = 0;

};