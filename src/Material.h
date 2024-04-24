#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

class Material {
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float exponent;
	bool reflective;

	Material();
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float exponent, bool reflective);
};

#endif