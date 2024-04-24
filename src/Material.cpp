#include "Material.h"

Material::Material() :
	ambient(0.0f, 0.0f, 0.0f),
	diffuse(0.0f, 0.0f, 0.0f),
	specular(0.0f, 0.0f, 0.0f),
	exponent(0.0f),
	reflective(false)
{}

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float exponent, bool reflective) :
	ambient(ambient),
	diffuse(diffuse),
	specular(specular),
	exponent(exponent),
	reflective(reflective)
{}